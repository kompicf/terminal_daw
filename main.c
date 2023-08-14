#include <stdlib.h>
#include "global.h"
#include "update.h"
#include "render.h"

enum {def, wave, key} program_state;

int main(int argc, char *argv[]) {
	// not used for now
	(void) argc;
	(void) argv;
	// ncurses init
  initscr();
  noecho();
  curs_set(FALSE);
	nodelay(stdscr, TRUE);
	raw();

	current_wave = malloc(32);

	update_init();
	play_init();
	render_default_init();
	load_keybinds();

	program_state = def;

	i8 running = 1;
  while (running) {
		update_all();

		if(input_key == keybinds.open_wavemaker){
			if(program_state == wave) program_state = def;
			else program_state = wave;
		}
		else if(input_key == keybinds.quit){
			if(program_state == wave) program_state = def;
			else running = 0;
		}
		else if(input_key == keybinds.open_keybind_editor){
			program_state = key;
		}

		switch(program_state){
			case def:
				render_default_refresh();
				break;
			case wave:
				wavemaker_refresh();
				break;
			case key:
				keybind_editor();
				program_state = def;
				break;
			default:
				break;
		}
    refresh();
  }

	play_stop();

  endwin();

  return 0;
}

