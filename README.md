taggrepper
==========

A simple program to match several tags of media files against
specified regular expressions. Full documentation present in the man
page.


Installation
------------

[![Build Status](https://travis-ci.org/kumanna/taggrepper.svg?branch=master)](https://travis-ci.org/kumanna/taggrepper)

First, download the latest tarball from the Download section in the
[[Home]] page, and extract it.  The program requires the development
headers and libraries for PCRE, libmagic, the Mad ID3 tag library and,
optionally, the Ogg Vorbis and FLAC development libraries. On a recent
Debian machine, the following should fetch these:

    sudo apt-get install libpcre3-dev libmagic-dev libid3tag-dev

(add libvorbis-dev if you have several Ogg Vorbis files as
well. Likewise, add libflac-dev for FLAC files).

Then, just do ./configure;make;sudo make install

Then, running taggrepper with -h should give you some help. But here
is a short demo. "-r" descends recursively into folders.

Examples
--------

To find all songs in the ~/AV/Music_collection/ which have "Rahman" in
their "Artist" tag:

    $ taggrepper -r -a Rahman ~/AV/Music_collection/
    /home/kumar/AV/Music_collection/Indian/Hindi/Lagaan/05_-_Chale_Chalo.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/03_-_Mausam_&_Escape.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/07_-_Liquid_Dance.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/11_-_Gangsta_Blues.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/08_-_Latika's_Theme.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/12_-_Dreams_On_Fire.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/06_-_Ringa_Ringa.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/13_-_Jai_Ho.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/02_-_Riots.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/10_-_Millionaire.mp3
    /home/kumar/AV/Music_collection/Indian/Tamil/Bombay/01_-_Antha Arabi.mp3

PCRE is Perl compatible regular expressions. Since this library offers
some nice features (about which you can find out yourself), you can do
many more things. For example, to do a case insensitive match of the
previous search. Note that SPB and Sanjay Subrahmanyan also come in
(SPB is spelt in some tags as Balasubrahmanyam).

    $ taggrepper -r -a "(?i)Rahman" ~/AV/Music_collection/
    /home/kumar/AV/Music_collection/Indian/Hindi/Lagaan/05_-_Chale_Chalo.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/03_-_Mausam_&_Escape.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/07_-_Liquid_Dance.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/11_-_Gangsta_Blues.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/08_-_Latika's_Theme.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/12_-_Dreams_On_Fire.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/06_-_Ringa_Ringa.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/13_-_Jai_Ho.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/02_-_Riots.mp3
    /home/kumar/AV/Music_collection/Indian/Hindi/Slumdog_Millionaire_-_Music_From_The_Motion_Picture/10_-_Millionaire.mp3
    /home/kumar/AV/Music_collection/Indian/Tamil/Moonlit/Paadu_Nilaave.mp3
    /home/kumar/AV/Music_collection/Indian/Tamil/Moonlit/10_-_S._P._Balasubrahmanyam_-_Nilavu_thoongum_neram.mp3
    /home/kumar/AV/Music_collection/Indian/Tamil/Moonlit/04_-_Track_04.mp3
    /home/kumar/AV/Music_collection/Indian/Tamil/Moonlit/03_-_Nilave_vaa.mp3
    /home/kumar/AV/Music_collection/Indian/Tamil/Bombay/01_-_Antha Arabi.mp3
    /home/kumar/AV/Music_collection/Indian/Tamil/Pudhiya_Mugam/12-S._P._Balasubramaniam,_Anupama_-_July_Matham.mp3
    /home/kumar/AV/Music_collection/Indian/Classical/The_Sanjay_Subrahmanyan_Show/episode_3_mix.mp3
    /home/kumar/AV/Music_collection/Indian/Classical/The_Sanjay_Subrahmanyan_Show/episode_6_mix.mp3
    /home/kumar/AV/Music_collection/Indian/Classical/The_Sanjay_Subrahmanyan_Show/episode_5_mix.mp3
    /home/kumar/AV/Music_collection/Indian/Classical/The_Sanjay_Subrahmanyan_Show/episode_10_mix.mp3
    /home/kumar/AV/Music_collection/Indian/Classical/The_Sanjay_Subrahmanyan_Show/episode_7_mix.mp3
    /home/kumar/AV/Music_collection/Indian/Classical/The_Sanjay_Subrahmanyan_Show/SSS_Episode_011.mp3
    /home/kumar/AV/Music_collection/Indian/Classical/The_Sanjay_Subrahmanyan_Show/episode_1_mix.mp3
    /home/kumar/AV/Music_collection/Indian/Classical/The_Sanjay_Subrahmanyan_Show/episode_9_mix.mp3
    /home/kumar/AV/Music_collection/Indian/Classical/The_Sanjay_Subrahmanyan_Show/episode_4_mix.mp3
    /home/kumar/AV/Music_collection/Indian/Classical/The_Sanjay_Subrahmanyan_Show/episode_8_mix.mp3
    /home/kumar/AV/Music_collection/Indian/Classical/The_Sanjay_Subrahmanyan_Show/episode_2_mix.mp3

Finally, Unni Menon's songs in Rhythm.

    $ taggrepper -r --artist "Unni.*Menon" --album "Rhythm" ~/AV/Music_collection/
    /home/kumar/AV/Music_collection/Indian/Tamil/Rhythm/01-Dheenthanana.mp3

Tricks
------

taggrepper is most useful for making a quick playlist. For example,
you can easily create an M3U file from taggrepper output (see
http://en.wikipedia.org/wiki/M3U for details).

taggrepper can also be used as a find replacement tool, along with
xargs. For example, you could use eyeD3 to modify tags as:

    taggrepper [options] -print0 [files] | xargs -0 eyeD3 <options>

Or you could call mplayer recursively on a listing, like

    taggrepper [options] -print0 [files] | xargs -0 mplayer

Conclusion
----------

Hope you can give me some feedback. I have to write some more
documentation and tune some finer points, but it should be useful
already, I hope.

Suggestions and patches welcome.

  Kumar Appaiah
  E-mail: a dot kumar, followed by at, and ending with alumni dot iitm dot ac dot in
