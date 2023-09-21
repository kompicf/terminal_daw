[I stopped working on this, but I might continue next summer.]

Ultra simple terminal 8bit tracker

dependencies:
  portaudio
  ncurses
compile and run:
  make

inspired by patatracker
not finished
this is my first real project

quick guide:
  wavemaker:
    o to enter/exit wavemaker
    wasd/hjkl to move
    enter to set sample (and move to right to make it more efficient)
    space to start/stop wave preview
    f to save wave to one of 8 places (press again to cancel)
    g to load wave from one of 8 places (press again to cancel)
  default/normal mode:
    q to exit, o to enter wavemaker
    wasd to move
    1 2 3 4 to switch channels
    y to increment, t to decrement global octave
    i to enter/exit insert mode:
      on first column zsxcfvgbnjmk to enter note (on qwerty keyboard image its a piano starting with A (z) and ending with G# (k))
      on first column space to clear note and move one place down
      on first column y to increase and t to decrease octave locally
      on second column 12345678 to set wave you made in wavemaker
      on third column 012345678 to set volume (0 == mute)
    r to enter/exit replace mode:
      same as insert mode, but goes back to normal mode when pitch is changed/cleared
  keybind editor: l
    set custom keybinds
    if you want default or something is broken remove config.txt
  command mode: /
    type a command and press enter
    example:
    > tempo 60
    (dont type > )
        
    current commands:
      tempo x: set tempo to x bpm

todo:
  [+]config file
  [+]option to change tempo
  use more than 32 notes
  save project
  export to wav
