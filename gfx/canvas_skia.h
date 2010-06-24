// Copyright (c) 2010 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef GFX_CANVAS_SKIA_H_
#define GFX_CANVAS_SKIA_H_

#include "base/basictypes.h"
#include "gfx/canvas.h"
#include "skia/ext/platform_canvas.h"

#if defined(OS_POSIX) && !defined(OS_MACOSX)
typedef struct _GdkPixbuf GdkPixbuf;
#endif

namespace gfx {

class Canvas;

// CanvasSkia is a SkCanvas subclass that provides a number of methods for
// common operations used throughout an application built using base/gfx and
// app/gfx.
//
// All methods that take integer arguments (as is used throughout views)
// end with Int. If you need to use methods provided by the superclass
// you'll need to do a conversion. In particular you'll need to use
// macro SkIntToScalar(xxx), or if converting from a scalar to an integer
// SkScalarRound.
//
// A handful of methods in this class are overloaded providing an additional
// argument of type SkXfermode::Mode. SkXfermode::Mode specifies how the
// source and destination colors are combined. Unless otherwise specified,
// the variant that does not take a SkXfermode::Mode uses a transfer mode
// of kSrcOver_Mode.
class CanvasSkia : public skia::PlatformCanvas,
                   public Canvas {
 public:
  // Creates an empty Canvas. Callers must use initialize before using the
  // canvas.
  CanvasSkia();

  CanvasSkia(int width, int height, bool is_opaque);

  virtual ~CanvasSkia();

  // Compute the size required to draw some text with the provided font.
  // Attempts to fit the text with the provided width and height. Increases
  // height and then width as needed to make the text fit. This method
  // supports multiple lines.
  static void SizeStringInt(const std::wstring& text, const gfx::Font& font,
                            int* width, int* height, int flags);

  // Returns the default text alignment to be used when drawing text on a
  // gfx::CanvasSkia based on the directionality of the system locale language.
  // This function is used by gfx::Canvas::DrawStringInt when the text alignment
  // is not specified.
  //
  // This function returns either gfx::Canvas::TEXT_ALIGN_LEFT or
  // gfx::Canvas::TEXT_ALIGN_RIGHT.
  static int DefaultCanvasTextAlignment();

#if defined(OS_POSIX) && !defined(OS_MACOSX)
  // Draw the pixbuf in its natural size at (x, y).
  void DrawGdkPixbuf(GdkPixbuf* pixbuf, int x, int y);
#endif

#ifdef OS_WIN  // Only implemented on Windows for now.
  // Draws text with a 1-pixel halo around it of the given color. It allows
  // ClearType to be drawn to an otherwise transparenct bitmap for drag images.
  // Drag images have only 1-bit of transparency, so we don't do any fancy
  // blurring.
  void DrawStringWithHalo(const std::wstring& text,
                          const gfx::Font& font,
                          const SkColor& text_color,
                          const SkColor& halo_color,
                          int x, int y, int w, int h, int flags);
#endif

  // Extracts a bitmap from the contents of this canvas.
  SkBitmap ExtractBitmap() const;

  // Overridden from Canvas2:
  virtual bool GetClipRect(gfx::Rect* clip_rect);
  virtual bool ClipRectInt(int x, int y, int w, int h);
  virtual bool IntersectsClipRectInt(int x, int y, int w, int h);
  virtual void TranslateInt(int x, int y);
  virtual void ScaleInt(int x, int y);
  virtual void FillRectInt(int x, int y, int w, int h,
                           const SkPaint& paint);
  virtual void FillRectInt(const SkColor& color, int x, int y, int w,
                           int h);
  virtual void DrawRectInt(const SkColor& color, int x, int y, int w,
                           int h);
  virtual void DrawRectInt(const SkColor& color, int x, int y, int w, int h,
                           SkXfermode::Mode mode);
  virtual void DrawLineInt(const SkColor& color, int x1, int y1, int x2,
                           int y2);
  virtual void DrawBitmapInt(const SkBitmap& bitmap, int x, int y);
  virtual void DrawBitmapInt(const SkBitmap& bitmap, int x, int y,
                             const SkPaint& paint);
  virtual void DrawBitmapInt(const SkBitmap& bitmap, int src_x, int src_y,
                             int src_w, int src_h, int dest_x, int dest_y,
                             int dest_w, int dest_h, bool filter);
  virtual void DrawBitmapInt(const SkBitmap& bitmap, int src_x, int src_y,
                             int src_w, int src_h, int dest_x, int dest_y,
                             int dest_w, int dest_h, bool filter,
                             const SkPaint& paint);
  virtual void DrawStringInt(const std::wstring& text, const gfx::Font& font,
                             const SkColor& color, int x, int y, int w,
                             int h);
  virtual void DrawStringInt(const std::wstring& text, const gfx::Font& font,
                             const SkColor& color,
                             const gfx::Rect& display_rect);
  virtual void DrawStringInt(const std::wstring& text, const gfx::Font& font,
                             const SkColor& color, int x, int y, int w, int h,
                             int flags);
  virtual void DrawFocusRect(int x, int y, int width, int height);
  virtual void TileImageInt(const SkBitmap& bitmap, int x, int y, int w, int h);
  virtual void TileImageInt(const SkBitmap& bitmap, int src_x, int src_y,
                            int dest_x, int dest_y, int w, int h);
  virtual CanvasSkia* AsCanvasSkia();
  virtual const CanvasSkia* AsCanvasSkia() const;

 private:
#if defined(OS_WIN)
  // Draws text with the specified color, font and location. The text is
  // aligned to the left, vertically centered, clipped to the region. If the
  // text is too big, it is truncated and '...' is added to the end.
  void DrawStringInt(const std::wstring& text, HFONT font,
                     const SkColor& color, int x, int y, int w, int h,
                     int flags);
#endif

  DISALLOW_COPY_AND_ASSIGN(CanvasSkia);
};

}  // namespace gfx;

#endif  // GFX_CANVAS_SKIA_H_
