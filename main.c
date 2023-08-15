#include <stdlib.h>
#include "modes.h"
#include "update.h"

enum {def, wave, key, cmd} program_state;

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
		else if(input_key == keybinds.open_command_line){
			program_state = cmd;
		}

		switch(program_state){
			case def: render_default_refresh(); break;
			case wave: wavemaker_refresh(); break;
			case key:
				keybind_editor();
				program_state = def;
				break;
			case cmd:
				render_default_refresh();
				handle_commands();
				program_state = def;
				break;
			default: break;
		}
    refresh();
  }

	play_stop();

  endwin();

  return 0;
}

