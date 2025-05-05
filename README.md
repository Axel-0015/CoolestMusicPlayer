# 🎵 CoolestMusicPlayer

A modern and stylish desktop music player built with **Qt 6**, featuring:

- 🎧 MP3 playback
- 💿 Auto-extracted album cover (including circular rotation effect)
- 🎚️ Custom-styled slider and controls
- 🔁 Play modes: Order, Random, Loop
- 📜 Auto-advance to next song when finished

---

## ✨ Features

- **MP3 File Loading**: Load songs from a local directory
- **Stylized UI**: Clean layout with translucent widgets and custom icons
- **Album Cover Display**: Extracts embedded cover from MP3 using `TagLib`
- **Rotating Album Art**: Circular, rotating cover art while music is playing
- **Play Modes**: Switch between ordered, random, and repeat playback
- **Auto-Next**: Automatically plays the next song when the current one ends
- **Progress Tracking**: Displays current time, total duration, and progress bar

---

## 🖥️ Screenshots

<img width="926" alt="image" src="https://github.com/user-attachments/assets/20e0bfe5-b917-4eed-8ec5-2f2071f15c1e" />

---

##🔧 Technologies
C++ / Qt 6

QMediaPlayer / QAudioOutput

TagLib (for album art metadata)

---

##📁 Setup
Install Qt 6

Install TagLib

Clone this project

Build in Qt Creator or use qmake from the terminal

---

##💡 Notes
Place .mp3 music files into the music/ folder inside the project

Cover image must be embedded into the MP3 file using ID3v2 APIC tag

---

##👨‍💻 Author
Axel Li | GitHub @Axel-0015


