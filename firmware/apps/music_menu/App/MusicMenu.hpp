#pragma once

#include <App/App.hpp>
#include <App/Menu.hpp>

class SongTheMiracle : public App  {
    public:
        SongTheMiracle() : redraw_(true),
            playing_(true), prev_playing_(false),
            position_ms_(0), prev_position_ms_(0),
            prev_time_sec_(0), timestamp_ms_(0) {};
        virtual ~SongTheMiracle() {};

        virtual void begin(void);
        virtual void loop(void);
        virtual void key_event(uint8_t key);
        virtual void draw(Adafruit_GFX &gfx);
        virtual void end(void);

    private:
        bool redraw_;
        bool playing_;
        bool prev_playing_;
        size_t position_ms_;
        size_t prev_position_ms_;
        int prev_time_sec_;
        uint32_t timestamp_ms_;
};

class SongsOfInnocenceAlbumMenu : public Menu  {
    public:
        SongsOfInnocenceAlbumMenu() {};
        virtual ~SongsOfInnocenceAlbumMenu() {};
        virtual void begin(BpodMenu &menu);

    private:
        SongTheMiracle song_the_miracle_;
};

class U2ArtistMenu : public Menu  {
    public:
        U2ArtistMenu() {};
        virtual ~U2ArtistMenu() {};
        virtual void begin(BpodMenu &menu);

    private:
        SongsOfInnocenceAlbumMenu songs_of_innocence_album_menu_;
};

class SongsMenu : public Menu  {
    public:
        SongsMenu() {};
        virtual ~SongsMenu() {};
        virtual void begin(BpodMenu &menu);

    private:
        SongTheMiracle song_the_miracle_;
};

class AlbumsMenu : public Menu  {
    public:
        AlbumsMenu() {};
        virtual ~AlbumsMenu() {};
        virtual void begin(BpodMenu &menu);

    private:
        SongsOfInnocenceAlbumMenu songs_of_innocence_album_menu_;
};

class ArtistsMenu : public Menu  {
    public:
        ArtistsMenu() {};
        virtual ~ArtistsMenu() {};
        virtual void begin(BpodMenu &menu);

    private:
        U2ArtistMenu u2_artist_menu_;
};

class MusicMenu : public Menu  {
    public:
        MusicMenu() {};
        virtual ~MusicMenu() {};
        virtual void begin(BpodMenu &menu);

    private:
        ArtistsMenu artists_menu_;
        AlbumsMenu albums_menu_;
        SongsMenu songs_menu_;
};
