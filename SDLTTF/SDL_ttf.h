/*
  SDL_ttf:  A companion library to SDL for working with TrueType (tm) fonts
  Copyright (C) 2001-2022 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/


/**
 *  \file SDL_ttf.h
 *
 *  Header file for SDL_ttf library
 *
 *  This library is a wrapper around the excellent FreeType 2.0 library,
 *  available at: http://www.freetype.org/
 *
 *  Note: In many places, SDL_ttf will say "glyph" when it means "code point."
 *  Unicode is hard, we learn as we go, and we apologize for adding to the
 *  confusion.
 *
 */
#ifndef SDL_TTF_H_
#define SDL_TTF_H_

#include <SDL2/SDL.h>
#include <SDL2/begin_code.h>

/* Set up for C function definitions, even when using C++ */
#ifdef __cplusplus
extern "C" {
#endif

/**
 * Printable format: "%d.%d.%d", MAJOR, MINOR, PATCHLEVEL
 */
#define SDL_TTF_MAJOR_VERSION   2
#define SDL_TTF_MINOR_VERSION   19
#define SDL_TTF_PATCHLEVEL      2

/**
 * This macro can be used to fill a version structure with the compile-time
 * version of the SDL_ttf library.
 */
#define SDL_TTF_VERSION(X)                          \
{                                                   \
    (X)->major = SDL_TTF_MAJOR_VERSION;             \
    (X)->minor = SDL_TTF_MINOR_VERSION;             \
    (X)->patch = SDL_TTF_PATCHLEVEL;                \
}

/**
 * Backwards compatibility
 */
#define TTF_MAJOR_VERSION   SDL_TTF_MAJOR_VERSION
#define TTF_MINOR_VERSION   SDL_TTF_MINOR_VERSION
#define TTF_PATCHLEVEL      SDL_TTF_PATCHLEVEL
#define TTF_VERSION(X)      SDL_TTF_VERSION(X)

#if SDL_TTF_MAJOR_VERSION < 3 && SDL_MAJOR_VERSION < 3
/**
 *  This is the version number macro for the current SDL_ttf version.
 *
 *  In versions higher than 2.9.0, the minor version overflows into
 *  the thousands digit: for example, 2.23.0 is encoded as 4300.
 *  This macro will not be available in SDL 3.x or SDL_ttf 3.x.
 *
 *  Deprecated, use SDL_TTF_VERSION_ATLEAST or SDL_TTF_VERSION instead.
 */
#define SDL_TTF_COMPILEDVERSION \
    SDL_VERSIONNUM(SDL_TTF_MAJOR_VERSION, SDL_TTF_MINOR_VERSION, SDL_TTF_PATCHLEVEL)
#endif /* SDL_TTF_MAJOR_VERSION < 3 && SDL_MAJOR_VERSION < 3 */

/**
 *  This macro will evaluate to true if compiled with SDL_ttf at least X.Y.Z.
 */
#define SDL_TTF_VERSION_ATLEAST(X, Y, Z) \
    ((SDL_TTF_MAJOR_VERSION >= X) && \
     (SDL_TTF_MAJOR_VERSION > X || SDL_TTF_MINOR_VERSION >= Y) && \
     (SDL_TTF_MAJOR_VERSION > X || SDL_TTF_MINOR_VERSION > Y || SDL_TTF_PATCHLEVEL >= Z))

/* Make sure this is defined (only available in newer SDL versions) */
#ifndef SDL_DEPRECATED
#define SDL_DEPRECATED
#endif

/**
 * This function gets the version of the dynamically linked SDL_ttf library.
 *
 * it should NOT be used to fill a version structure, instead you should use
 * the SDL_TTF_VERSION() macro.
 *
 * \returns SDL_ttf version
 */
extern DECLSPEC const SDL_version * SDLCALL TTF_Linked_Version(void);

/**
 * This function stores the version of the FreeType2 library in use.
 *
 * TTF_Init() should be called before calling this function.
 *
 * \param major pointer to get the major version number
 * \param minor pointer to get the minor version number
 * \param patch pointer to get the param version number
 *
 * \sa TTF_Init
 */
extern DECLSPEC void SDLCALL TTF_GetFreeTypeVersion(int *major, int *minor, int *patch);

/**
 * This function stores the version of the HarfBuzz library in use, or 0 if
 * HarfBuzz is not available.
 *
 * \param major pointer to get the major version number
 * \param minor pointer to get the minor version number
 * \param patch pointer to get the param version number
 */
extern DECLSPEC void SDLCALL TTF_GetHarfBuzzVersion(int *major, int *minor, int *patch);

/**
 * ZERO WIDTH NO-BREAKSPACE (Unicode byte order mark)
 */
#define UNICODE_BOM_NATIVE  0xFEFF
#define UNICODE_BOM_SWAPPED 0xFFFE

/**
 * This function tells the library whether UNICODE text is generally
 * byteswapped.
 *
 * A UNICODE BOM character in a string will override this setting for the
 * remainder of that string.
 *
 * \param swapped boolean to indicate whether text is byteswapped
 */
extern DECLSPEC void SDLCALL TTF_ByteSwappedUNICODE(SDL_bool swapped);

/**
 * The internal structure containing font information
 */
typedef struct _TTF_Font TTF_Font;

/**
 * Initialize the TTF engine
 *
 * \returns 0 if successful, -1 on error
 *
 * \sa TTF_Quit
 */
extern DECLSPEC int SDLCALL TTF_Init(void);

/**
 * Open a font file and create a font of the specified point size.
 *
 * Some .fon fonts will have several sizes embedded in the file, so the point
 * size becomes the index of choosing which size. If the value is too high,
 * the last indexed size will be the default.
 *
 * \param file file name
 * \param ptsize point size
 * \returns a valid TTF_Font, NULL on error
 *
 * \sa TTF_CloseFont
 */
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFont(const char *file, int ptsize);
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontIndex(const char *file, int ptsize, long index);

/**
 * Open a font file from a SDL_RWops.
 *
 * \param src SDL_RWops to use. 'src' must be kept alive for the lifetime of
 *            the TTF_Font.
 * \param freesrc can be set so that TTF_CloseFont closes the RWops
 * \param ptsize point size
 * \returns a valid TTF_Font, NULL on error
 *
 * \sa TTF_CloseFont
 */
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontRW(SDL_RWops *src, int freesrc, int ptsize);
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontIndexRW(SDL_RWops *src, int freesrc, int ptsize, long index);

/**
 * Opens a font using the given horizontal and vertical target resolutions (in
 * DPI).
 *
 * DPI scaling only applies to scalable fonts (e.g. TrueType).
 *
 * \param file file name
 * \param ptsize point size
 * \param hdpi horizontal DPI
 * \param vdpi vertical DPI
 * \returns a valid TTF_Font, NULL on error
 *
 * \sa TTF_CloseFont
 */
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontDPI(const char *file, int ptsize, unsigned int hdpi, unsigned int vdpi);
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontIndexDPI(const char *file, int ptsize, long index, unsigned int hdpi, unsigned int vdpi);
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontDPIRW(SDL_RWops *src, int freesrc, int ptsize, unsigned int hdpi, unsigned int vdpi);
extern DECLSPEC TTF_Font * SDLCALL TTF_OpenFontIndexDPIRW(SDL_RWops *src, int freesrc, int ptsize, long index, unsigned int hdpi, unsigned int vdpi);

/**
 * Set font size dynamically.
 *
 * This clears already generated glyphs, if any, from the cache.
 *
 * \param font TTF_Font handle
 * \param ptsize point size
 * \returns 0 if successful, -1 on error
 */
extern DECLSPEC int SDLCALL TTF_SetFontSize(TTF_Font *font, int ptsize);

/**
 * Set font size dynamically.
 *
 * This clears already generated glyphs, if any, from the cache.
 *
 * \param font TTF_Font handle
 * \param ptsize point size
 * \param hdpi horizontal DPI
 * \param vdpi vertical DPI
 * \returns 0 if successful, -1 on error.
 */
extern DECLSPEC int SDLCALL TTF_SetFontSizeDPI(TTF_Font *font, int ptsize, unsigned int hdpi, unsigned int vdpi);

/**
 * Font style flags
 */
#define TTF_STYLE_NORMAL        0x00
#define TTF_STYLE_BOLD          0x01
#define TTF_STYLE_ITALIC        0x02
#define TTF_STYLE_UNDERLINE     0x04
#define TTF_STYLE_STRIKETHROUGH 0x08

/**
 * Retrieve the font style.
 *
 * \param font TTF_Font handle
 * \returns font style
 *
 * \sa TTF_SetFontStyle
 */
extern DECLSPEC int SDLCALL TTF_GetFontStyle(const TTF_Font *font);

/**
 * Set the font style.
 *
 * Setting the style clears already generated glyphs, if any, from the cache.
 *
 * \param font TTF_Font handlea
 * \param style style flags OR'ed
 *
 * \sa TTF_GetFontStyle
 */
extern DECLSPEC void SDLCALL TTF_SetFontStyle(TTF_Font *font, int style);

/**
 * Retrieve the font outline.
 *
 * \param font TTF_Font handle
 * \returns font outline
 *
 * \sa TTF_SetFontOutline
 */
extern DECLSPEC int SDLCALL TTF_GetFontOutline(const TTF_Font *font);

/**
 * Set the font outline.
 *
 * \param font TTF_Font handle
 * \param outline positive outline value, 0 to default
 *
 * \sa TTF_GetFontOutline
 */
extern DECLSPEC void SDLCALL TTF_SetFontOutline(TTF_Font *font, int outline);


/**
 * Hinting flags
 */
#define TTF_HINTING_NORMAL          0
#define TTF_HINTING_LIGHT           1
#define TTF_HINTING_MONO            2
#define TTF_HINTING_NONE            3
#define TTF_HINTING_LIGHT_SUBPIXEL  4

/**
 * Retrieve FreeType hinter setting.
 *
 * \param font TTF_Font handle
 * \returns hinting flag
 *
 * \sa TTF_SetFontHinting
 */
extern DECLSPEC int SDLCALL TTF_GetFontHinting(const TTF_Font *font);

/**
 * Set FreeType hinter settings.
 *
 * Setting it clears already generated glyphs, if any, from the cache.
 *
 * \param font TTF_Font handle
 * \param hinting hinting flag
 *
 * \sa TTF_GetFontHinting
 */
extern DECLSPEC void SDLCALL TTF_SetFontHinting(TTF_Font *font, int hinting);

/**
 * Special layout option for rendering wrapped text
 */
#define TTF_WRAPPED_ALIGN_LEFT     0
#define TTF_WRAPPED_ALIGN_CENTER   1
#define TTF_WRAPPED_ALIGN_RIGHT    2

/**
 * Get wrap alignment option
 *
 * \param font TTF_Font handle
 * \returns wrap alignment option
 *
 * \sa TTF_SetFontWrappedAlign
 */
extern DECLSPEC int SDLCALL TTF_GetFontWrappedAlign(const TTF_Font *font);

/**
 * Set wrap alignment option
 *
 * \param font TTF_Font handle
 * \param align wrap alignment option
 *
 * \sa TTF_GetFontWrappedAlign
 */
extern DECLSPEC void SDLCALL TTF_SetFontWrappedAlign(TTF_Font *font, int align);

/**
 * Get the total height of the font - usually equal to point size
 *
 * \param font TTF_Font handle
 * \returns font height
 */
extern DECLSPEC int SDLCALL TTF_FontHeight(const TTF_Font *font);

/**
 * Get the offset from the baseline to the top of the font This is a positive
 * value, relative to the baseline.
 *
 * \param font TTF_Font handle
 * \returns font ascent
 */
extern DECLSPEC int SDLCALL TTF_FontAscent(const TTF_Font *font);

/**
 * Get the offset from the baseline to the bottom of the font This is a
 * negative value, relative to the baseline.
 *
 * \param font TTF_Font handle
 * \returns font descent
 */
extern DECLSPEC int SDLCALL TTF_FontDescent(const TTF_Font *font);

/**
 * Get the recommended spacing between lines of text for this font
 *
 * \param font TTF_Font handle
 * \returns spacing value
 */
extern DECLSPEC int SDLCALL TTF_FontLineSkip(const TTF_Font *font);

/**
 * Get whether or not kerning is allowed for this font
 *
 * \param font TTF_Font handle
 * \returns tell is kerning is enabled
 */
extern DECLSPEC int SDLCALL TTF_GetFontKerning(const TTF_Font *font);

/**
 * Set if kerning is allowed for this font
 *
 * \param font TTF_Font handle
 * \param allowed enable or not kerning
 */
extern DECLSPEC void SDLCALL TTF_SetFontKerning(TTF_Font *font, int allowed);

/**
 * Get the number of faces of the font
 *
 * \param font TTF_Font handle
 * \returns number of FreeType font faces
 */
extern DECLSPEC long SDLCALL TTF_FontFaces(const TTF_Font *font);

/**
 * Tell whether it is a fixed width font.
 *
 * \param font TTF_Font handle
 * \returns 1 if true, 0 if not, -1 on error
 */
extern DECLSPEC int SDLCALL TTF_FontFaceIsFixedWidth(const TTF_Font *font);

/**
 * Get font family name
 *
 * \param font TTF_Font handle
 * \returns font family name, NULL on error
 */
extern DECLSPEC char * SDLCALL TTF_FontFaceFamilyName(const TTF_Font *font);

/**
 * Get font style name
 *
 * \param font TTF_Font handle
 * \returns font style name, NULL on error
 */
extern DECLSPEC char * SDLCALL TTF_FontFaceStyleName(const TTF_Font *font);

/**
 * Check wether a glyph is provided by the font or not
 *
 * \param font TTF_Font handle
 * \param ch char index, 16bits
 * \returns 1 if provided
 *
 * \sa TTF_GlyphIsProvided32
 */
extern DECLSPEC int SDLCALL TTF_GlyphIsProvided(TTF_Font *font, Uint16 ch);

/**
 * Check wether a glyph is provided by the font or not
 *
 * \param font TTF_Font handle
 * \param ch char index, 32bits
 * \returns 1 if provided
 *
 * \sa TTF_GlyphIsProvided
 */
extern DECLSPEC int SDLCALL TTF_GlyphIsProvided32(TTF_Font *font, Uint32 ch);

/**
 * Get the metrics (dimensions) of a glyph To understand what these metrics
 * mean, here is a useful link:
 * http://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
 *
 * \param font TTF_Font handle
 * \param ch char index, 16bits
 *
 * \sa TTF_GlyphIsProvided32
 */
extern DECLSPEC int SDLCALL TTF_GlyphMetrics(TTF_Font *font, Uint16 ch,
                     int *minx, int *maxx,
                     int *miny, int *maxy, int *advance);

/**
 * Get the metrics (dimensions) of a glyph To understand what these metrics
 * mean, here is a useful link:
 * http://freetype.sourceforge.net/freetype2/docs/tutorial/step2.html
 *
 * \param font TTF_Font handle
 * \param ch char index, 32bits
 *
 * \sa TTF_GlyphMetrics
 */
extern DECLSPEC int SDLCALL TTF_GlyphMetrics32(TTF_Font *font, Uint32 ch,
                     int *minx, int *maxx,
                     int *miny, int *maxy, int *advance);

/**
 * Get the dimensions of a rendered string of text
 *
 * \param font TTF_Font handle
 * \param text text to render
 * \param w output width
 * \param h output height
 * \returns 0 if successful, -1 on error
 *
 * \sa TTF_SizeText
 * \sa TTF_SizeUTF8
 * \sa TTF_SizeUNICODE
 */
extern DECLSPEC int SDLCALL TTF_SizeText(TTF_Font *font, const char *text, int *w, int *h);
extern DECLSPEC int SDLCALL TTF_SizeUTF8(TTF_Font *font, const char *text, int *w, int *h);
extern DECLSPEC int SDLCALL TTF_SizeUNICODE(TTF_Font *font, const Uint16 *text, int *w, int *h);

/**
 * Get the measurement string of text without rendering e.g.
 *
 * the number of characters that can be rendered before reaching
 * 'measure_width'
 *
 * \param font TTF_Font handle
 * \param text text to render
 * \param measure_width in pixels to measure this text (input)
 * \param count number of characters that can be rendered (output)
 * \param extent latest calculated width (output)
 * \returns 0 if successful, -1 on error
 *
 * \sa TTF_MeasureText
 * \sa TTF_MeasureUTF8
 * \sa TTF_MeasureUNICODE
 */
extern DECLSPEC int SDLCALL TTF_MeasureText(TTF_Font *font, const char *text, int measure_width, int *extent, int *count);
extern DECLSPEC int SDLCALL TTF_MeasureUTF8(TTF_Font *font, const char *text, int measure_width, int *extent, int *count);
extern DECLSPEC int SDLCALL TTF_MeasureUNICODE(TTF_Font *font, const Uint16 *text, int measure_width, int *extent, int *count);

/**
 * Create an 8-bit palettized surface and render the given text at fast
 * quality with the given font and color.
 *
 * The 0 pixel is the colorkey, giving a transparent background, and the 1
 * pixel is set to the text color.
 *
 * \param font TTF_Font handle
 * \param text text to render
 * \param fg foreground color
 * \returns the new surface, or NULL if there was an error.
 *
 * \sa TTF_RenderText_Solid
 * \sa TTF_RenderUTF8_Solid
 * \sa TTF_RenderUNICODE_Solid
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Solid(TTF_Font *font,
                const char *text, SDL_Color fg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Solid(TTF_Font *font,
                const char *text, SDL_Color fg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Solid(TTF_Font *font,
                const Uint16 *text, SDL_Color fg);

/**
 * Create an 8-bit palettized surface and render the given text at fast
 * quality with the given font and color.
 *
 * The 0 pixel is the colorkey, giving a transparent background, and the 1
 * pixel is set to the text color. Text is wrapped to multiple lines on line
 * endings and on word boundaries if it extends beyond wrapLength in pixels.
 * If wrapLength is 0, only wrap on new lines.
 *
 * \param font TTF_Font handle
 * \param text text to render
 * \param fg foreground color
 * \param wrapLength wrap length
 * \returns the new surface, or NULL if there was an error.
 *
 * \sa TTF_RenderText_Solid_Wrapped
 * \sa TTF_RenderUTF8__Wrapped
 * \sa TTF_RenderUNICODE_Solid_Wrapped
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Solid_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, Uint32 wrapLength);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Solid_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, Uint32 wrapLength);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Solid_Wrapped(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, Uint32 wrapLength);

/**
 * Create an 8-bit palettized surface and render the given glyph at fast
 * quality with the given font and color.
 *
 * The 0 pixel is the colorkey, giving a transparent background, and the 1
 * pixel is set to the text color. The glyph is rendered without any padding
 * or centering in the X direction, and aligned normally in the Y direction.
 * This function returns the new surface, or NULL if there was an error.
 *
 * \param font TTF_Font handle
 * \param ch character index
 * \param fg foreground color
 * \returns the new surface, or NULL if there was an error.
 *
 * \sa TTF_RenderGlyph_Solid
 * \sa TTF_RenderGlyph32_Solid
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph_Solid(TTF_Font *font,
                    Uint16 ch, SDL_Color fg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph32_Solid(TTF_Font *font,
                    Uint32 ch, SDL_Color fg);

/**
 * Create an 8-bit palettized surface and render the given text at high
 * quality with the given font and colors.
 *
 * The 0 pixel is background, while other pixels have varying degrees of the
 * foreground color. This function returns the new surface, or NULL if there
 * was an error.
 *
 * \param font TTF_Font handle
 * \param text text to render
 * \param fg foreground color
 * \param bg background color
 * \returns the new surface, or NULL if there was an error.
 *
 * \sa TTF_RenderText_Shaded
 * \sa TTF_RenderUTF8_Shaded
 * \sa TTF_RenderUNICODE_Shaded
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Shaded(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Shaded(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Shaded(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, SDL_Color bg);

/**
 * Create an 8-bit palettized surface and render the given text at high
 * quality with the given font and colors.
 *
 * The 0 pixel is background, while other pixels have varying degrees of the
 * foreground color. Text is wrapped to multiple lines on line endings and on
 * word boundaries if it extends beyond wrapLength in pixels. If wrapLength is
 * 0, only wrap on new lines. This function returns the new surface, or NULL
 * if there was an error.
 *
 * \param font TTF_Font handle
 * \param text text to render
 * \param fg foreground color
 * \param bg background color
 * \param wrapLength wrap length
 * \returns the new surface, or NULL if there was an error.
 *
 * \sa TTF_RenderText_Shaded_Wrapped
 * \sa TTF_RenderUTF8_Shaded_Wrapped
 * \sa TTF_RenderUNICODE_Shaded_Wrapped
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Shaded_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Shaded_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Shaded_Wrapped(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);

/**
 * Create an 8-bit palettized surface and render the given glyph at high
 * quality with the given font and colors.
 *
 * The 0 pixel is background, while other pixels have varying degrees of the
 * foreground color. The glyph is rendered without any padding or centering in
 * the X direction, and aligned normally in the Y direction. This function
 * returns the new surface, or NULL if there was an error.
 *
 * \param font TTF_Font handle
 * \param ch character index
 * \param fg foreground color
 * \param bg background color
 * \returns the new surface, or NULL if there was an error.
 *
 * \sa TTF_RenderGlyph_Shaded
 * \sa TTF_RenderGlyph32_Shaded
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph_Shaded(TTF_Font *font,
                Uint16 ch, SDL_Color fg, SDL_Color bg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph32_Shaded(TTF_Font *font,
                Uint32 ch, SDL_Color fg, SDL_Color bg);

/**
 * Create a 32-bit ARGB surface and render the given text at high quality,
 * using alpha blending to dither the font with the given color.
 *
 * This function returns the new surface, or NULL if there was an error.
 *
 * \param font TTF_Font handle
 * \param text text to render
 * \param fg foreground color
 * \returns the new surface, or NULL if there was an error.
 *
 * \sa TTF_RenderText_Blended
 * \sa TTF_RenderUTF8_Blended
 * \sa TTF_RenderUNICODE_Blended
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Blended(TTF_Font *font,
                const char *text, SDL_Color fg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Blended(TTF_Font *font,
                const char *text, SDL_Color fg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Blended(TTF_Font *font,
                const Uint16 *text, SDL_Color fg);


/**
 * Create a 32-bit ARGB surface and render the given text at high quality,
 * using alpha blending to dither the font with the given color.
 *
 * Text is wrapped to multiple lines on line endings and on word boundaries if
 * it extends beyond wrapLength in pixels. If wrapLength is 0, only wrap on
 * new lines. This function returns the new surface, or NULL if there was an
 * error.
 *
 * \param font TTF_Font handle
 * \param text text to render
 * \param fg foreground color
 * \param wrapLength wrap length
 * \returns the new surface, or NULL if there was an error.
 *
 * \sa TTF_RenderText_Blended_Wrapped
 * \sa TTF_RenderUTF8_Blended_Wrapped
 * \sa TTF_RenderUNICODE_Blended_Wrapped
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_Blended_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, Uint32 wrapLength);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_Blended_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, Uint32 wrapLength);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_Blended_Wrapped(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, Uint32 wrapLength);

/**
 * Create a 32-bit ARGB surface and render the given glyph at high quality,
 * using alpha blending to dither the font with the given color.
 *
 * The glyph is rendered without any padding or centering in the X direction,
 * and aligned normally in the Y direction. This function returns the new
 * surface, or NULL if there was an error.
 *
 * \param font TTF_Font handle
 * \param ch character index
 * \param fg foreground color
 * \returns the new surface, or NULL if there was an error.
 *
 * \sa TTF_RenderGlyph_Blended
 * \sa TTF_RenderGlyph32_Blended
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph_Blended(TTF_Font *font,
                        Uint16 ch, SDL_Color fg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph32_Blended(TTF_Font *font,
                        Uint32 ch, SDL_Color fg);

/**
 * Create a 32-bit surface (SDL_PIXELFORMAT_ARGB8888) and render the given
 * text using FreeType LCD rendering, with the given font and colors.
 *
 * This function returns the new surface, or NULL if there was an error.
 *
 * \param font TTF_Font handle
 * \param text text to render
 * \param fg foreground color
 * \param bg background color
 * \returns the new surface, or NULL if there was an error.a
 *
 * \sa TTF_RenderText_LCD
 * \sa TTF_RenderUTF8_LCD
 * \sa TTF_RenderUNICODE_LCD
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_LCD(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_LCD(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_LCD(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, SDL_Color bg);

/**
 * Create a 32-bit surface (SDL_PIXELFORMAT_ARGB8888) and render the given
 * text using FreeType LCD rendering, with the given font and colors.
 *
 * Text is wrapped to multiple lines on line endings and on word boundaries if
 * it extends beyond wrapLength in pixels. This function returns the new
 * surface, or NULL if there was an error.
 *
 * \param font TTF_Font handle
 * \param text text to render
 * \param fg foreground color
 * \param bg background color
 * \param wrapLength wrap length
 * \returns the new surface, or NULL if there was an error.
 *
 * \sa TTF_RenderText_LCD_Wrapped
 * \sa TTF_RenderUTF8_LCD_Wrapped
 * \sa TTF_RenderUNICODE_LCD_Wrapped
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderText_LCD_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUTF8_LCD_Wrapped(TTF_Font *font,
                const char *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderUNICODE_LCD_Wrapped(TTF_Font *font,
                const Uint16 *text, SDL_Color fg, SDL_Color bg, Uint32 wrapLength);

/**
 * Create a 32-bit surface (SDL_PIXELFORMAT_ARGB8888) and render the given
 * text using FreeType LCD rendering, with the given font and colors.
 *
 * The glyph is rendered without any padding or centering in the X direction,
 * and aligned normally in the Y direction. This function returns the new
 * surface, or NULL if there was an error.
 *
 * \param font TTF_Font handle
 * \param ch character index
 * \param fg foreground color
 * \param bg background color
 * \returns the new surface, or NULL if there was an error.
 *
 * \sa TTF_RenderGlyph_LCD
 * \sa TTF_RenderGlyph32_LCD
 */
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph_LCD(TTF_Font *font,
                Uint16 ch, SDL_Color fg, SDL_Color bg);
extern DECLSPEC SDL_Surface * SDLCALL TTF_RenderGlyph32_LCD(TTF_Font *font,
                Uint32 ch, SDL_Color fg, SDL_Color bg);


/* For compatibility with previous versions, here are the old functions */
#define TTF_RenderText(font, text, fg, bg)  \
    TTF_RenderText_Shaded(font, text, fg, bg)
#define TTF_RenderUTF8(font, text, fg, bg)  \
    TTF_RenderUTF8_Shaded(font, text, fg, bg)
#define TTF_RenderUNICODE(font, text, fg, bg)   \
    TTF_RenderUNICODE_Shaded(font, text, fg, bg)

/**
 * Close an opened font file
 *
 * \param font TTF_Font handle
 *
 * \sa TTF_OpenFontIndexDPIRW
 * \sa TTF_OpenFontRW
 * \sa TTF_OpenFontDPI
 * \sa TTF_OpenFontDPIRW
 * \sa TTF_OpenFontIndex
 * \sa TTF_OpenFontIndexDPI
 * \sa TTF_OpenFontIndexDPIRW
 * \sa TTF_OpenFontIndexRW
 */
extern DECLSPEC void SDLCALL TTF_CloseFont(TTF_Font *font);

/**
 * De-initialize the TTF engine
 *
 * \sa TTF_Init
 */
extern DECLSPEC void SDLCALL TTF_Quit(void);

/**
 * Check if the TTF engine is initialized
 *
 * \returns number of initialization
 *
 * \sa TTF_Init
 * \sa TTF_Quit
 */
extern DECLSPEC int SDLCALL TTF_WasInit(void);

/**
 * Get the kerning size of two glyphs indices.
 *
 * DEPRECATED: this function requires FreeType font indexes, not glyphs,
 * by accident, which we don't expose through this API, so it could give
 * wildly incorrect results, especially with non-ASCII values.
 * Going forward, please use TTF_GetFontKerningSizeGlyphs() instead, which
 *  does what you probably expected this function to do.
 */
extern DECLSPEC int TTF_GetFontKerningSize(TTF_Font *font, int prev_index, int index) SDL_DEPRECATED;

/**
 * Get the kerning size of two glyphs
 *
 * \param font TTF_Font handle
 * \param previous_ch previous char index, 16bits
 * \param ch          next char index, 16bits
 *
 * \returns 0 if successful, -1 on error
 */
extern DECLSPEC int TTF_GetFontKerningSizeGlyphs(TTF_Font *font, Uint16 previous_ch, Uint16 ch);

/**
 * Get the kerning size of two glyphs
 *
 * \param font TTF_Font handle
 * \param previous_ch previous char index, 32bits
 * \param ch          next char index, 32bits
 *
 * \returns 0 if successful, -1 on error
 */
extern DECLSPEC int TTF_GetFontKerningSizeGlyphs32(TTF_Font *font, Uint32 previous_ch, Uint32 ch);

/**
 * Enable Signed Distance Field rendering (with the Blended APIs)
 *
 * \param font TTF_Font handle
 * \param on_off boolean on/off
 *
 * \returns 0 if successful, -1 on error
 *
 * \sa TTF_GetFontSDF
 */
extern DECLSPEC int TTF_SetFontSDF(TTF_Font *font, SDL_bool on_off);

/**
 * Tell wether Signed Distance Field rendering is enabled
 *
 * \param font TTF_Font handle
 *
 * \returns boolean on/off
 *
 * \sa TTF_SetFontSDF
 */
extern DECLSPEC SDL_bool TTF_GetFontSDF(const TTF_Font *font);

/**
 * Report SDL_ttf errors
 *
 * \sa TTF_GetError
 */
#define TTF_SetError    SDL_SetError

/**
 * Get last SDL_ttf error
 *
 * \sa TTF_SetError
 */
#define TTF_GetError    SDL_GetError

/**
 * Direction flags
 *
 * \sa TTF_SetFontDirection
 */
typedef enum
{
  TTF_DIRECTION_LTR = 0, /* Left to Right */
  TTF_DIRECTION_RTL,     /* Right to Left */
  TTF_DIRECTION_TTB,     /* Top to Bottom */
  TTF_DIRECTION_BTT      /* Bottom to Top */
} TTF_Direction;

/**
 * Set Direction to be used for text shaping.
 *
 * \returns 0, or -1 if SDL_ttf is not compiled with HarfBuzz
 *
 *          This function is deprecated. Prefer TTF_SetFontDirection().
 *
 * \sa TTF_SetFontDirection
 */
extern DECLSPEC int SDLCALL TTF_SetDirection(int direction); /* hb_direction_t */

/**
 * Set Script to be used for text shaping.
 *
 * \returns 0, or -1 if SDL_ttf is not compiled with HarfBuzz
 *
 *          This function is deprecated. Prefer TTF_SetFontScriptName().
 *
 * \sa TTF_SetFontScriptName
 */
extern DECLSPEC int SDLCALL TTF_SetScript(int script); /* hb_script_t */

/**
 * Set direction per font.
 *
 * It overrides the global direction set with the deprecated
 * TTF_SetDirection().
 *
 * \param font TTF_Font handle
 * \param direction TTF_Direction parameter
 * \returns 0, or -1 if SDL_ttf is not compiled with HarfBuzz or invalid
 *          parameter
 */
extern DECLSPEC int SDLCALL TTF_SetFontDirection(TTF_Font *font, TTF_Direction direction);

/**
 * Set script per font.
 *
 * It overrides the global script set with the deprecated TTF_SetScript().
 *
 * \param font TTF_Font handle
 * \param script null terminated string of exactly 4 characters.
 * \returns 0, or -1 if SDL_ttf is not compiled with HarfBuzz or invalid
 *          parameter
 */
extern DECLSPEC int SDLCALL TTF_SetFontScriptName(TTF_Font *font, const char *script);

/* Ends C function definitions when using C++ */
#ifdef __cplusplus
}
#endif
#include <SDL2/close_code.h>

#endif /* SDL_TTF_H_ */

/* vi: set ts=4 sw=4 expandtab: */
