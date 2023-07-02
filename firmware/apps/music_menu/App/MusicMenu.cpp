#include "MusicMenu.hpp"

#include <Images/BpodTitleBar.hpp>
#include <Images/BpodProgressBar.hpp>
#include <Images/U2AlbumArt.hpp>
#include <Music/u2_snippet.hpp>

#include <stringdb.h>

#define SONG_DURATION_MS (9500)

U2Snippet u2_snippet;

void SongTheMiracle::begin(void)
{
    redraw_ = true;
    playing_ = true;
    prev_playing_ = false;
    position_ms_ = 0;
    prev_position_ms_ = 0;
    prev_time_sec_ = 0;
    timestamp_ms_ = (uint32_t)millis();
}

void SongTheMiracle::loop(void)
{
    uint32_t current_ms = (uint32_t)millis();
    if ( playing_ && ( current_ms > timestamp_ms_ ) )
    {
        position_ms_ += ( current_ms - timestamp_ms_ );
        if ( position_ms_ > SONG_DURATION_MS )
        {
            position_ms_ = SONG_DURATION_MS;
            playing_ = false;
        }
    }
    timestamp_ms_ = current_ms;
}

void SongTheMiracle::key_event(uint8_t key)
{
    switch( key )
    {
        case APP_KEY_PLAY:
            if ( !playing_ && (position_ms_ == SONG_DURATION_MS) )
            {
                position_ms_ = 0;
            }
            playing_ = !playing_;
            break;

        case APP_KEY_SCROLL_CLOCKWISE:
            if ( position_ms_ <= (SONG_DURATION_MS - 200) )
            {
                position_ms_ += 200;
            }
            else
            {
                position_ms_ = SONG_DURATION_MS;
            }
            break;

        case APP_KEY_SCROLL_ANTICLOCKWISE:
            if ( position_ms_ >= 200 )
            {
                position_ms_ -= 200;
            }
            else
            {
                position_ms_ = 0;
            }
            break;

        case APP_KEY_MENU:
            App::manager_end();
            break;
    }
}

void SongTheMiracle::draw(Adafruit_GFX &gfx)
{
    size_t y = 0;
    char buf[128];
    size_t progress = 0;
    bool redraw_title = false;
    bool redraw_song = false;
    bool redraw_progress = false;
    bool redraw_time = false;
    int time_sec = 0;
    int time_duration_sec = (SONG_DURATION_MS / 1000);

    if ( redraw_ )
    {
        redraw_ = false;
        printf(STRING(CONSOLE_CLEAR));
        gfx.fillRect(0, BpodTitleBar::view_y(gfx), gfx.width(), BpodTitleBar::view_height(gfx), 0x8410);
        redraw_title = true;
        redraw_song = true;
        redraw_progress = true;
        redraw_time = true;
    }
    if ( prev_playing_ != playing_ )
    {
        prev_playing_ = playing_;
        redraw_title = true;
    }
    if ( position_ms_ != prev_position_ms_ )
    {
        prev_position_ms_ = position_ms_;
        redraw_progress = true;
    }
    time_sec = (position_ms_/ 1000);
    if ( prev_time_sec_ != time_sec )
    {
        prev_time_sec_ = time_sec;
        redraw_time = true;
    }

    if ( redraw_title )
    {
        // Title
        STRING_CACHE();
        if ( playing_ )
        {
            BpodTitleBar::draw(gfx, STRING(STRING_NOW_PLAYING), true);
            sprintf(buf, STRING(FMT_TITLE), STRING(STRING_NOW_PLAYING));
        }
        else
        {
            BpodTitleBar::draw(gfx, STRING(STRING_PAUSE), true);
            sprintf(buf, STRING(FMT_TITLE), STRING(STRING_PAUSE));
        }
        STRING_CLEAR();
        if ( strlen(buf) > 0 )
        {
            buf[strlen(buf) - 1] = '\0';
            memset(&buf[strlen(buf)], ' ', sizeof(buf) - 1 - strlen(buf));
            buf[sizeof(buf) - 1] = '\0';
        }
        printf(STRING(CONSOLE_CURSOR_STRING), 1, 1, buf);
        y += BpodTitleBar::height();
        y += 4;
    }
    else
    {
        y += BpodTitleBar::height();
        y += 4;
    }

    if ( redraw_song )
    {
        // Cover art
        U2AlbumArt::draw((gfx.width() / 2) - (U2AlbumArt::width / 2), y, gfx);
        y += U2AlbumArt::height + 4;

        // Song
        gfx.setTextColor(0xffff);
        gfx.setTextSize(1);
        gfx.setCursor((gfx.width() / 2) - ((STRING_STRLEN(STRING_THE_MIRACLE) * 6) / 2), y);
        gfx.print(STRING(STRING_THE_MIRACLE));
        STRING_CACHE();
        printf(STRING(FMT_LINE), STRING(STRING_THE_MIRACLE));
        STRING_CLEAR();
        y += 8 + 2;

        // Album
        gfx.setTextColor(0xffff);
        gfx.setTextSize(1);
        gfx.setCursor((gfx.width() / 2) - ((STRING_STRLEN(STRING_SONGS_OF_INNOCENCE) * 6) / 2), y);
        gfx.print(STRING(STRING_SONGS_OF_INNOCENCE));
        STRING_CACHE();
        printf(STRING(FMT_LINE), STRING(STRING_SONGS_OF_INNOCENCE));
        STRING_CLEAR();
        y += 8 + 2;
    }
    else
    {
        y += U2AlbumArt::height + 4;
        y += 8 + 2;
        y += 8 + 2;
    }

    if ( redraw_progress )
    {
        // Progress bar
        y += 4;
        progress = (position_ms_ * 100) / SONG_DURATION_MS;
        BpodProgressBar::draw(gfx, 10, y, 110, 0, progress, 100);
        printf(STRING(CONSOLE_CURSOR_PERCENT), 4, 1, progress);
        y += BpodProgressBar::height() + 6;
    }
    else
    {
        y += 4;
        y += BpodProgressBar::height() + 6;
    }

    if ( redraw_time )
    {
        // Clear for text
        gfx.fillRect(0, y, gfx.width(), gfx.height() - y, 0x8410);

        // Time start
        sprintf(buf, STRING(FMT_TIME), ' ', time_sec);
        gfx.setTextColor(0xffff);
        gfx.setTextSize(1);
        gfx.setCursor(8, y);
        gfx.print(buf);
        printf(STRING(CONSOLE_CURSOR_STRING), 5, 1, buf);

        // Time end
        sprintf(buf, STRING(FMT_TIME), '-', time_duration_sec - time_sec);
        gfx.setTextColor(0xffff);
        gfx.setTextSize(1);
        gfx.setCursor(gfx.width() - 8 - (strlen(buf) * 6), y);
        gfx.print(buf);
        printf(STRING(CONSOLE_CURSOR_STRING), 6, 1, buf);
    }
}

void SongTheMiracle::end(void)
{

}

void SongsOfInnocenceAlbumMenu::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_SONGS_OF_INNOCENCE));
    menu.add(STRING(STRING_THE_MIRACLE), [this](){ App::manager_begin(song_the_miracle_); });
}

void U2ArtistMenu::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_U2));
    menu.add(STRING(STRING_SONGS_OF_INNOCENCE), [this](){ App::manager_begin(songs_of_innocence_album_menu_); });
}

void ArtistsMenu::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_ARTISTS));
    menu.add(STRING(STRING_U2), [this](){ App::manager_begin(u2_artist_menu_); });
}

void AlbumsMenu::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_ALBUMS));
    menu.add(STRING(STRING_SONGS_OF_INNOCENCE), [this](){ App::manager_begin(songs_of_innocence_album_menu_); });
}

void SongsMenu::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_SONGS));
    menu.add(STRING(STRING_THE_MIRACLE), [this](){ App::manager_begin(song_the_miracle_); });
}

void MusicMenu::begin(BpodMenu &menu) {
    menu.set_title(STRING(STRING_MUSIC));
    menu.add(STRING(STRING_ARTISTS), [this](){ App::manager_begin(artists_menu_); });
    menu.add(STRING(STRING_ALBUMS), [this](){ App::manager_begin(albums_menu_); });
    menu.add(STRING(STRING_SONGS), [this](){ App::manager_begin(songs_menu_); });
}
