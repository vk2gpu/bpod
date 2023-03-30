#include "TextView.hpp"

#include <Images/BpodTitleBar.hpp>
#include <Images/BpodScrollBar.hpp>

ssize_t VWRC_API viewerc_read(void *ctx, size_t offset, char *text, size_t text_size)
{
    return reinterpret_cast<TextView*>(ctx)->read(offset, text, text_size);
}

void VWRC_API viewerc_calc_string_view(void *ctx, const char *text, size_t *width, size_t *height)
{
    return reinterpret_cast<TextView*>(ctx)->calc_string_view(text, width, height);
}

void VWRC_API viewerc_draw_string(void *ctx, size_t x, size_t y, const char *text)
{
    return reinterpret_cast<TextView*>(ctx)->draw_string(x, y, text);
}

ssize_t TextView::read(size_t offset, char *text, size_t text_size)
{
    if ( offset >= this->text_.size() )
    {
        return 0;
    }
    if ( (offset + text_size ) > this->text_.size() )
    {
        text_size = this->text_.size() - offset;
    }
    memcpy(text, this->text_.c_str() + offset, text_size);
    return text_size;
}

void TextView::calc_string_view(const char *text, size_t *width, size_t *height)
{
    *height = 8;
    *width = strlen(text) * 6;
}

void TextView::draw_string(size_t x, size_t y, const char *text)
{
    Adafruit_GFX &gfx = *draw_gfx;
    if ( !draw_gfx )
    {
        return;
    }
    x += 1;
    y += 1 + BpodTitleBarBackground::height;
    gfx.setCursor(x, y);
    gfx.print(text);
}

void TextView::begin(void)
{
    vwrc_init(this->viewerc_, sizeof(this->viewerc_));
    vwrc_set_calc_string_view(this->viewerc_, viewerc_calc_string_view, this);
    vwrc_set_draw_string(this->viewerc_, viewerc_draw_string, this);
    draw_gfx = nullptr;
    redraw_ = true;
    redraw_text_ = true;
    scroll_bar_ = false;
    text_width_ = 0;
    text_height_ = 0;
}

void TextView::key_event(uint8_t key)
{
    size_t prev_row = 0;
    size_t row = 0;
    switch( key )
    {
        case APP_KEY_SCROLL_CLOCKWISE:
            vwrc_get_row(this->viewerc_, &prev_row);
            scroll_by_page_ ? vwrc_scroll_down_page(this->viewerc_) : vwrc_scroll_down(this->viewerc_);
            vwrc_get_row(this->viewerc_, &row);
            redraw_text_ = row != prev_row;
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
            vwrc_get_row(this->viewerc_, &prev_row);
            scroll_by_page_ ? vwrc_scroll_up_page(this->viewerc_) : vwrc_scroll_up(this->viewerc_);
            vwrc_get_row(this->viewerc_, &row);
            redraw_text_ = row != prev_row;
            break;

        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void TextView::draw(Adafruit_GFX &gfx)
{
    draw_gfx = &gfx;
    size_t row = 0;
    size_t rows = 0;
    size_t rows_per_view = 0;
    if ( redraw_ )
    {
        redraw_ = false;
        redraw_text_ = true;
        vwrc_get_row(this->viewerc_, &row);
        text_width_ = gfx.width() - 2;
        text_height_ = BpodTitleBar::view_height(gfx) - 2;
        BpodTitleBar::draw(gfx, title_);

        vwrc_set_text(this->viewerc_, this->text_.size(), viewerc_read, this);
        vwrc_set_view(this->viewerc_, text_width_, text_height_);
        vwrc_get_row_count(this->viewerc_, &rows);
        vwrc_get_rows_per_view(this->viewerc_, &rows_per_view);
        if ( rows_per_view < rows )
        {
            // need a scroll bar
            scroll_bar_ = true;
            text_width_ = text_width_ - BpodScrollBar::width();
            vwrc_set_view(this->viewerc_, text_width_, text_height_);
        }
        else
        {
            // no scroll bar (text fits)
            scroll_bar_ = false;
        }
        vwrc_scroll_to_row(this->viewerc_, row);
    }
    if ( redraw_text_ )
    {
        redraw_text_ = false;
        if ( scroll_bar_ )
        {
            vwrc_get_row(this->viewerc_, &row);
            vwrc_get_row_count(this->viewerc_, &rows);
            vwrc_get_rows_per_view(this->viewerc_, &rows_per_view);
            BpodScrollBar::draw(gfx, BpodTitleBar::view_y(gfx), row, rows_per_view, rows);
        }
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), (int16_t)text_width_ + 2, (int16_t)text_height_ + 2, 0xffff);
        gfx.setTextColor(0x0000);
        gfx.setTextSize(1, 1);
        vwrc_draw_view(this->viewerc_);
    }
    draw_gfx = nullptr;
}

void TextView::end(void)
{
    vwrc_fini(this->viewerc_);
}
