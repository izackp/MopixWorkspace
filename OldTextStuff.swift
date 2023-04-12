//
//  TextLine.swift
//  TestGame
//
//  Created by Isaac Paul on 10/19/22.
//

import ICU
import SDL2
import SDL2_ttf

struct SpaceInfo {
    var count:Int
    var width:Int
    
    static let zero = SpaceInfo(count: 0, width: 0)
    
    mutating func increment(_ width:Int) {
        count += 1
        self.width += width
    }
}

struct StringWidth {
    init(start: SpaceInfo, end: SpaceInfo, width: Int, metrics:[CharacterMetrics]) {
        self.start = start
        self.end = end
        self.width = width
        self.metrics = metrics
    }
    
    let start:SpaceInfo
    let end:SpaceInfo
    let width:Int
    let metrics:[CharacterMetrics]
    
    static let zero = StringWidth(start: SpaceInfo.zero, end: SpaceInfo.zero, width: 0, metrics: [])
    
    init(width:Int) {
        start = SpaceInfo.zero
        end = SpaceInfo.zero
        self.width = width
        metrics = []
    }
}

//Character spacing == tracking
struct CharacterMetrics {
    var width:Int
    var advance:Int
    
    static let zero = CharacterMetrics(width: 0, advance: 0)
    
    func copyTo(_ other: inout CharacterMetrics) {
        other.width = width
        other.advance = advance
    }
}


struct TextLine {
    internal init(str: Substring, height: Int, widthInfo: StringWidth) {
        self.str = str
        self.height = height
        self.widthInfo = widthInfo
    }
    
    internal init(str: Substring, height: Int, width: Int) {
        self.str = str
        self.height = height
        self.widthInfo = StringWidth(width: width)
    }
    
    let str:Substring
    let height:Int
    //let startIndex:Int //Cache to avoid walking string to get equivalent index
    let widthInfo:StringWidth
    
    func append(endIndex:String.Index, endWidthInfo:StringWidth) -> TextLine {
        let info = StringWidth(start: widthInfo.start, end: endWidthInfo.end, width: widthInfo.width + endWidthInfo.width, metrics: widthInfo.metrics + endWidthInfo.metrics)
        return TextLine(str: str[str.startIndex..<endIndex], height: height, widthInfo: info)
    }
    
    func strTrimmingEndSpace() -> Substring {
        let offset = str.index(str.endIndex, offsetBy: -widthInfo.end.count)
        let newStr = str[str.startIndex..<offset]
        return newStr
    }
    
    func widthTrimmingEndSpace() -> Int {
        return widthInfo.width - widthInfo.end.width
    }
    
    func strTrimmingSpace() -> Substring {
        let startOffset = str.index(str.startIndex, offsetBy: widthInfo.start.count)
        let offset = str.index(str.endIndex, offsetBy: -widthInfo.end.count)
        let newStr = str[startOffset..<offset]
        return newStr
    }
    
    func widthTrimmingSpace() -> Int {
        return widthInfo.width - widthInfo.start.width - widthInfo.end.width
    }
    
    func indexOfPosition(_ xPos:Int) -> Int {
        var curPos = 0
        var index = 0
        for cInfo in widthInfo.metrics {
            curPos += cInfo.advance
            let outerSpace = (cInfo.advance - cInfo.width)
            let centerOfGlyph = curPos - outerSpace - (cInfo.width / 2)
            if (xPos < centerOfGlyph) {
                return index
            }
            index += 1
        }
        return index
    }
    
    func indexOfPositionInTrim(_ xPos:Int) -> Int {
        var curPos = 0
        var index = 0
        for cInfo in widthInfo.metrics[widthInfo.start.count..<widthInfo.metrics.count-widthInfo.end.count] {
            curPos += cInfo.advance
            let outerSpace = (cInfo.advance - cInfo.width)
            let centerOfGlyph = curPos - outerSpace - (cInfo.width / 2)
            if (xPos < centerOfGlyph) {
                return index
            }
            index += 1
        }
        return index
    }
}

struct TextLine2 {
    internal init(str: [RenderableCharacter], height: Int, widthInfo: StringWidth) {
        self.characters = str
        self.maxHeight = height
        self.widthInfo = widthInfo
    }
    
    internal init(str: [RenderableCharacter], height: Int, width: Int) {
        self.characters = str
        self.maxHeight = height
        self.widthInfo = StringWidth(width: width)
    }
    
    let characters:[RenderableCharacter]
    let maxHeight:Int
    let baseLinePos:Int
    let widthInfo:StringWidth
    
}

/*
 let link: AttributeScopes.FoundationAttributes.LinkAttribute

 public let morphology: AttributeScopes.FoundationAttributes.MorphologyAttribute
 GrammaticalGender, PartOfSpeech, GrammaticalNumber

 public let inflect: AttributeScopes.FoundationAttributes.InflectionRuleAttribute
 InflectionRule: Automatic, specific
 
 public let languageIdentifier: AttributeScopes.FoundationAttributes.LanguageIdentifierAttribute
 String
 
 public let personNameComponent: AttributeScopes.FoundationAttributes.PersonNameComponentAttribute
 givenName, familyname, etc
 
 public let numberFormat: AttributeScopes.FoundationAttributes.NumberFormatAttributes
 numberSymbol, numberPart
 
 public let dateField: AttributeScopes.FoundationAttributes.DateFieldAttribute
 era, year, etc
 
 public let inlinePresentationIntent: AttributeScopes.FoundationAttributes.InlinePresentationIntentAttribute
 below
 
 public let presentationIntent: AttributeScopes.FoundationAttributes.PresentationIntentAttribute
 List of: paragraph header(level: Int) orderedList unorderedList listItem(ordinal: Int) codeBlock(languageHint: String?) blockQuote thematicBreak table(columns: [PresentationIntent.TableColumn]) tableHeaderRow tableRow(rowIndex: Int) tableCell(columnIndex: Int)
 
 public let alternateDescription: AttributeScopes.FoundationAttributes.AlternateDescriptionAttribute
 string
 
 public let imageURL: AttributeScopes.FoundationAttributes.ImageURLAttribute
 Url
 
 public let replacementIndex: AttributeScopes.FoundationAttributes.ReplacementIndexAttribute
 Int... ??
 
 public let measurement: AttributeScopes.FoundationAttributes.MeasurementAttribute
 value, unit
 
 public let inflectionAlternative: AttributeScopes.FoundationAttributes.InflectionAlternativeAttribute
 Attributed string
 
 public let byteCount: AttributeScopes.FoundationAttributes.ByteCountAttribute
 case value
 case spelledOutValue
 case unit(AttributeScopes.FoundationAttributes.ByteCountAttribute.Unit)
 case actualByteCount
}
 */

/*
 * font
 *        public let foregroundColor: AttributeScopes.MopixAttributes.ForegroundColorAttribute
 
 public let backgroundColor: AttributeScopes.MopixAttributes.BackgroundColorAttribute

 public let ligature: AttributeScopes.MopixAttributes.LigatureAttribute

 public let kern: AttributeScopes.MopixAttributes.KernAttribute //Individual spacing

 public let tracking: AttributeScopes.MopixAttributes.TrackingAttribute //spacing over range of characters

 public let strikethroughStyle: AttributeScopes.MopixAttributes.StrikethroughStyleAttribute

 public let underlineStyle: AttributeScopes.MopixAttributes.UnderlineStyleAttribute

 public let strokeColor: AttributeScopes.MopixAttributes.StrokeColorAttribute

 public let strokeWidth: AttributeScopes.MopixAttributes.StrokeWidthAttribute
 public let shadow: AttributeScopes.MopixAttributes.ShadowAttribute
 */

struct TextContext {
    let font:FontDesc
    let foregroundColor:LabeledColor
    let backgroundColor:LabeledColor
    let kern:Float
    let tracking:Float
}

extension LabeledColor {
    static func from(_ value:UInt32?) -> LabeledColor? {
        guard let value = value else { return nil }
        return LabeledColor(integerLiteral: value)
    }
}

extension TextContext {
    func absorbAttributes(_ attributes:ScopedAttributeContainer<AttributeScopes.MopixAttributes>) -> TextContext {
        let font = attributes.font ?? self.font
        let foregroundColor = LabeledColor.from(attributes.foregroundColor) ?? self.foregroundColor
        let backgroundColor = LabeledColor.from(attributes.backgroundColor) ?? self.backgroundColor
        let kern = attributes.kern ?? 0
        let tracking = attributes.tracking ?? 0
        return TextContext(font: font, foregroundColor: foregroundColor, backgroundColor: backgroundColor, kern: kern, tracking: tracking)
    }
}

public enum LineBreak : Hashable, Codable {
    case soft
    case hard
}

public struct RenderableCharacter {
    let img:Image? //Empty for whitespace
    let size:Size<Int>
    let baseLine:Int
    let isWhitespace:Bool
}

//https://developer.apple.com/documentation/foundation/nsattributedstring/key/1533563-foregroundcolor
/*
 Do we have segments?
 or do we have an array of each char?
 */
extension LineBreakCursor {
    func isHardBreak() -> Bool {
        if case .hard = self.ruleStatus {
            return true
        }
        return false
    }
}
extension TextLine {
    static func buildFrom(_ str:AttributedString, renderContext:UIRenderContext, context:TextContext, maxWidthPxs:Int? = nil) throws -> [TextLine2] {
        
        //var fontMap:[FontDesc:Font] = [:]
        var width:Int = 0
        var start = str.startIndex
        var lastEnd = str.startIndex
        var end = str.startIndex
        
        var maxLines = 99
        
        var resultLines:[TextLine2] = []
        var lastLine:TextLine2? = nil
        
        let allCharacters = String(str.characters[...]) //https://forums.swift.org/t/attributedstring-to-string/61667
        let lineCursor = LineBreakCursor(text: allCharacters)
        var position:Int? = 0
        var positionIndex:String.Index? = lineCursor.first()
        // We can turn the below into an iterator to clean up some code
        if let nextLineBreak = lineCursor.next(), let positionIndexLive = positionIndex {
            position! += allCharacters.distance(from: positionIndexLive, to: nextLineBreak)
            positionIndex = nextLineBreak
        } else {
            position = nil
            positionIndex = nil
        }
        var hardBreak = lineCursor.isHardBreak()
        
        
        var currentPos = 0
        
        var atStart = true
        var startSpaceInfo = SpaceInfo.zero
        var endSpaceInfo = SpaceInfo.zero
        for run in str.runs {
            let runRange = run.range
            let atPoint = runRange.lowerBound == runRange.upperBound
            let test:ScopedAttributeContainer<AttributeScopes.MopixAttributes> = run.mopix
            let overwrittenContext = context.absorbAttributes(test)
            let characters = str.characters[run.range]
            let font = try renderContext.fetchFont(overwrittenContext.font)
            let height = font._font.height()
            
            //walking str might be cheaper
            var characterMetrics:[RenderableCharacter] = []
            for c in characters {
                var canLineBreak = false //Gotta ignore skipped whitespace too
                if let livePosition = position, let positionIndexLive = positionIndex {
                    let isLineBreak = currentPos == livePosition
                    canLineBreak = isLineBreak
                    if let nextLineBreak = lineCursor.next() {
                        position = livePosition + allCharacters.distance(from: positionIndexLive, to: nextLineBreak)
                        positionIndex = nextLineBreak
                        hardBreak = lineCursor.isHardBreak()
                    } else {
                        position = nil
                        positionIndex = nil
                    }
                }
                let metrics = try font._font.glyphMetrics(c: c)
                let thisWidth = metrics.advance + Int(overwrittenContext.kern) + Int(overwrittenContext.tracking)
                width += thisWidth
                let cSize = Size<Int>(thisWidth, metrics.frame.height)
                if (c.isWhitespace) {
                    if (atStart) {
                        startSpaceInfo.increment(thisWidth)
                    } else {
                        endSpaceInfo.increment(thisWidth)
                    }
                    
                    let rc = RenderableCharacter(img: nil, size: cSize, baseLine: 0)
                    characterMetrics.append(rc)
                } else {
                    atStart = false
                    endSpaceInfo = SpaceInfo.zero
                    
                    let image = try font.glyph(c)
                    let rc = RenderableCharacter(img: image, size: cSize, baseLine: font._font.descent())
                    characterMetrics.append(rc)
                }
            }
            
            
            //let widthInfo = try analyzeStringMetrics(font, Substring(characters), characterSpacing: 0)
            
            //let width = widthInfo.width
            if let maxWidthPxs = maxWidthPxs {
                if let line = lastLine {
                    let segmentDoesntFit = line.widthInfo.width + width > maxWidthPxs
                    if (segmentDoesntFit) {
                        resultLines.append(line)
                        lastLine = nil
                        if (resultLines.count >= maxLines) { break }
                        let start = line.str.startIndex
                        let newLine = TextLine(str: text[line.str.endIndex..<nextLineBreak], height: height, widthInfo: widthInfo)
                        lastLine = newLine
                    } else {
                        lastLine = line.append(endIndex: nextLineBreak, endWidthInfo: widthInfo)
                    }
                } else {
                    let newLine = TextLine(str: subStr, height: height, widthInfo: widthInfo)
                    lastLine = newLine
                }
            } else {
                
            }
            previousLineBreak = nextLineBreak
            startIndex += widthInfo.metrics.count
            for eachC in characters {
                
                
            }
            
        }
        return []
    }
}
/*
 if let textStyle = run.inlinePresentationIntent {
     /*
      public static var emphasized: InlinePresentationIntent { get }
      public static var stronglyEmphasized: InlinePresentationIntent { get }
      public static var code: InlinePresentationIntent { get }
      public static var strikethrough: InlinePresentationIntent { get }
      public static var softBreak: InlinePresentationIntent { get }
      public static var lineBreak: InlinePresentationIntent { get }
      public static var inlineHTML: InlinePresentationIntent { get }
      public static var blockHTML: InlinePresentationIntent { get }
      */

     // 6
     if textStyle.contains(.stronglyEmphasized) {
       print("Text is Bold")
     }
     if textStyle.contains(.emphasized) {
       print("Text is Italic")
     }
   // 7
   } else {
     print("Text is Regular")
   }
 */

func splitIntoLines(_ font:SDLFont, _ text:String, maxWidthPxs:Int) throws -> [TextLine] {
    var lines:[TextLine] = []
    var width:Int = 0
    var count:Int = 0
    var start:Int = 0
    let height = font.height()
    for (i, c) in text.enumerated() {
        do {
            let metrics = try font.glyphMetrics(c: c)
            if (width + metrics.advance > maxWidthPxs) {
                let line = TextLine(str: text[start...i], height: height, width: width)
                lines.append(line)
                start = i
                width = 0
                count = 0
            }
            width += metrics.advance
            count += 1
        } catch {
            print("Error couldn't measure character '\(c)': \(error.localizedDescription)")
        }
    }
    if (count > 0) {
        let line = TextLine(str: text[start...start + count], height: height, width: width)
        lines.append(line)
    }
    return lines
}

//Welcome to one of the ugliest functions in this code base
//Builds a list of lines used to layout text
//These lines will not include any initial whitespace unless:
//  - The previous line was ended with a line break
//  - It is the beginning of the line
//Overall mimicing behavior of uilabel from iOS
//Also does not include any whitespace at the end of the line
//Lines are broken up by word to fit inside maxWidthPxs
//They broken up even further by character if they still do not fit
//Line breaking is much more complex then I though: https://www.unicode.org/reports/tr14/
//
func splitIntoLinesWordWrapped(_ font:SDLFont, _ text:String, maxWidthPxs:Int, characterSpacing:Int) throws -> [TextLine] {
    var lines:[TextLine] = []
    let charPos:[Int] = Array(repeating: 0, count: text.count)
    var width:Int = 0
    let test:Character = "\u{0001e834}"
    var startIndex:String.Index = text.startIndex
    var lastWord:Substring? = nil
    var len = 0
    let height:Int = font.height()
    for eachWord in text.iterateWords() {
        var wordWidth:Int = 0
        var lastItemWasNewline = false
        var lastItemWasSkipped = false
        var spacesInCur = 0
        var spaceWidth = 0
        for c in eachWord {
            len += 1
            if (c == "\n") {
                if let lastWord2 = lastWord {
                    let line = TextLine(str: text[startIndex..<lastWord2.endIndex], height: height, width: width)
                    lines.append(line)
                    let dist = text.distance(from: startIndex, to: lastWord2.endIndex)
                    len -= dist
                    width = 0
                    startIndex = lastWord2.endIndex
                    lastWord = nil
                } else {
                    let endIndex = text.index(startIndex, offsetBy: len)
                    let line = TextLine(str: text[startIndex..<endIndex], height: height, width: width)
                    lines.append(line)
                    startIndex = eachWord.endIndex
                    len = 0
                    width = 0
                }
                lastItemWasNewline = true
                continue
            }
            if (!lastItemWasNewline && lastWord == nil && c.isWhitespace) {
                lastItemWasSkipped = true
                continue
            } else if (!lastItemWasNewline && c.isWhitespace) {
                spacesInCur += 1
            }
            lastItemWasNewline = false
            if lastItemWasSkipped {
                lastItemWasSkipped = false
                startIndex = text.index(startIndex, offsetBy: len - 1)
                len = 1
            }
            do {
                let metrics = try font.glyphMetrics(c: c)
                let nextWidth = wordWidth + metrics.advance + characterSpacing
                
                let wordDoesntFit = width + nextWidth > maxWidthPxs
                if (wordDoesntFit) {
                    if let lastWord2 = lastWord {
                        
                        let line = TextLine(str: text[startIndex..<lastWord2.endIndex], height: height, width: width)
                        lines.append(line)
                        let dist = text.distance(from: startIndex, to: lastWord2.endIndex) + 1
                        len -= dist
                        let index = text.index(lastWord2.endIndex, offsetBy: spacesInCur)
                        startIndex = index
                        width = 0
                        wordWidth = nextWidth - spaceWidth
                        lastWord = nil
                    } else {
                        
                        let endIndex = text.index(startIndex, offsetBy: len-1)
                        let line = TextLine(str: text[startIndex..<endIndex], height: height, width: wordWidth)
                        lines.append(line)
                        startIndex = endIndex
                        len = 1
                        width = 0
                        wordWidth = metrics.advance + characterSpacing
                    }
                } else {
                    wordWidth = nextWidth
                    if (c.isWhitespace) {
                        spaceWidth += metrics.advance + characterSpacing
                    }
                }
            } catch {
                print("Error couldn't measure character '\(c)': \(error.localizedDescription)")
            }
        }
        width += wordWidth
        lastWord = eachWord
    }
    if let lastWord = lastWord {
        let line = TextLine(str: text[startIndex..<lastWord.endIndex], height: height, width: width)
        lines.append(line)
    }
    return lines
}


//TODO:
//The ICU implementation is _heavy_. It requires a utf16 string while swift uses
//utf8 backed strings. When returning string indexes this means we have to convert
//index to the other.. which requires walking the string. I also suspect the wrapper framework
//does cache this utf16 portion while iterating the string

func splitIntoLinesWordWrapped2(_ font:SDLFont, _ text:String, maxWidthPxs:Int, characterSpacing:Int, maxLines:Int = 256) throws -> [TextLine] {
    var result:[TextLine] = []
    let height:Int = font.height()
    let lineCursor = LineBreakCursor(text: text)
    var previousLineBreak = lineCursor.first()
    var lastLine:TextLine? = nil
    var startIndex = 0
    //TODO: Note we are not handling hard breaks
    while let nextLineBreak = lineCursor.next() {
        
        let subStr = text[previousLineBreak..<nextLineBreak]
        let widthInfo = try analyzeStringMetrics(font, subStr, characterSpacing: characterSpacing)
        
        let width = widthInfo.width
        if let line = lastLine {
            let segmentDoesntFit = line.widthInfo.width + width > maxWidthPxs
            if (segmentDoesntFit) {
                result.append(line)
                lastLine = nil
                if (result.count >= maxLines) { break }
                let start = line.str.startIndex
                let newLine = TextLine(str: text[line.str.endIndex..<nextLineBreak], height: height, widthInfo: widthInfo)
                lastLine = newLine
            } else {
                lastLine = line.append(endIndex: nextLineBreak, endWidthInfo: widthInfo)
            }
        } else {
            let newLine = TextLine(str: subStr, height: height, widthInfo: widthInfo)
            lastLine = newLine
        }
        previousLineBreak = nextLineBreak
        startIndex += widthInfo.metrics.count
    }
    if let lastLine = lastLine {
        result.append(lastLine)
    }
   
    return result
}


func analyzeStringMetrics(_ font:SDLFont, _ str:Substring, characterSpacing:Int) throws -> StringWidth {
    var width = 0
    var atStart = true
    var start = SpaceInfo.zero
    var end = SpaceInfo.zero
    
    var i = 0
    let count = str.count //walking string but cheaper than reallocating an array
    var characterMetrics:[CharacterMetrics] = Array(repeating: CharacterMetrics.zero, count: count)
    for c in str {
        let metrics = try font.glyphMetrics(c: c)
        let thisWidth = metrics.advance + characterSpacing //TODO: We can squeeze more in a line if we check to fit without including this.
        width += thisWidth
        characterMetrics[i] = CharacterMetrics(width: metrics.frame.width, advance: metrics.advance)
        if (c.isWhitespace) {
            if (atStart) {
                start.increment(thisWidth)
            } else {
                end.increment(thisWidth)
            }
        } else {
            atStart = false
            end = SpaceInfo.zero
        }
        i+=1
    }
    return StringWidth(start: start, end: end, width: width, metrics: characterMetrics)
}
