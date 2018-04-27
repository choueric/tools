#include <stdio.h>
#include <ctype.h>
#include <unistd.h>
#include <getopt.h>
#include <png.h>
#include <ft2build.h>
#include FT_FREETYPE_H

struct Image {
	unsigned w, h;
	char *data;
};

struct Glyph {
	unsigned code;
	struct Image image;
	unsigned x, y;
	int offset_x;
	int offset_y;
	int advance;
};

static void copy_bmp_data(struct Image *image, FT_Bitmap *bmp)
{
	char *data = image->data;
	unsigned h = image->h;
	unsigned w = image->w;
	int x, y;

	if (bmp->pitch < 0) {
		for (y = 0; y < bmp->rows; ++y)
			for (x = 0; x < bmp->width; ++x)
				data[x + (h - 1 - y) * w] = bmp->buffer[x - y * bmp->pitch];
	} else {
		for (y = 0; y < bmp->rows; ++y)
			for (x = 0; x < bmp->width; ++x)
				data[x + y * w] = bmp->buffer[x + y * bmp->pitch];
	}
}

static const char *pixel_mode_str(FT_Pixel_Mode mode)
{
	switch(mode) {
		case FT_PIXEL_MODE_NONE: return "NONE";
		case FT_PIXEL_MODE_MONO: return "MONO";
		case FT_PIXEL_MODE_GRAY: return "GRAY";
		case FT_PIXEL_MODE_GRAY2: return "GRAY2";
		case FT_PIXEL_MODE_GRAY4: return "GRAY4";
		case FT_PIXEL_MODE_LCD: return "LCD";
		case FT_PIXEL_MODE_LCD_V: return "LCD_V";
		case FT_PIXEL_MODE_BGRA: return "BGRA";
		default: return "UNKNOWN";
	}
}

static int fetch_glyph(FT_Face face, unsigned i, struct Glyph *glyph)
{
	int flags = FT_LOAD_FORCE_AUTOHINT, err;
	FT_Bitmap *bmp;
	unsigned n;

	n = FT_Get_Char_Index(face, i);
	if (!n) {
		fprintf(stderr, "Warning: Glyph index is 0.\n");
		return -1;
	}

	err = FT_Load_Glyph(face, n, flags);
	if (err) {
		fprintf(stderr, "FT_Load_Glyph fail: %d\n", err);
		return err;
	}

#if 1
	err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
#else
	err = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_MONO);
#endif
	if (err) {
		fprintf(stderr, "FT_Render_Glyph fail: %d\n", err);
		return err;
	}
	bmp = &face->glyph->bitmap;

	printf("-------------------------------------------------\n");
	printf("  Char %u('%c'): glyph %u, size %dx%d\n", i, i, n, bmp->width, bmp->rows);
	printf("  pixel mode = %s\n", pixel_mode_str(bmp->pixel_mode));

	glyph->code = i;
	glyph->image.w = bmp->width;
	glyph->image.h = bmp->rows;
	glyph->image.data = (char *)malloc(bmp->width * bmp->rows);
	glyph->offset_x = face->glyph->bitmap_left;
	glyph->offset_y = face->glyph->bitmap_top - bmp->rows;
	glyph->advance = (int)(face->glyph->advance.x + 32) / 64;

	copy_bmp_data(&glyph->image, bmp);

	return 0;
}

static int render_glyph(struct Glyph *glyph)
{
	struct Image *image = &glyph->image;
	char *data = image->data;
	unsigned w = image->w;
	unsigned h = image->h;
	int x, y;

	for (y = 0; y < h; ++y) {
		for (x = 0; x < w; ++x) {
			if (data[x + y * w])
				printf("*");
			else
				printf(" ");
		}
		printf("\n");
	}

	return 0;
}

int main(int argc, char **argv)
{
	FT_Library library;
	FT_Face face;
	int err, size = 50;
	struct Glyph glyph;

	err = FT_Init_FreeType(&library);
	if (err) {
		fprintf(stderr, "Couldn't initialize library library\n");
		return 1;
	}

	err = FT_New_Face(library, "./UbuntuMono-B.ttf", 0, &face);
	if (err) {
		fprintf(stderr, "Couldn't load font file\n");
		if (err == FT_Err_Unknown_File_Format)
			fprintf(stderr, "Unknown file format\n");
		return 1;
	}

	const char *name = FT_Get_Postscript_Name(face);
	printf("Font name  : %s\n", name);
	printf("Glyphs num : %ld\n", face->num_glyphs);

	err = FT_Set_Pixel_Sizes(face, 0, size);
	if (err) {
		fprintf(stderr, "Couldn't set size\n");
		return 1;
	}

	err = fetch_glyph(face, 'A', &glyph);
	if (err) {
		fprintf(stderr, "Couldn't process glyph: %d\n", err);
		return 1;
	}

	render_glyph(&glyph);

	free(glyph.image.data);
	FT_Done_Face(face);
	FT_Done_FreeType(library);

	return 0;
}
