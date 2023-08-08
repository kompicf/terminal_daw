#include <stdlib.h>
#include "update.h"
#include "render.h"

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

	i8 wavemaker = 0;
	i8 running = 1;
  while (running) {
		update_all();

		switch (input_key){
			case 'o':
				wavemaker = ~wavemaker;
				break;
			case 'q':
				if(wavemaker) wavemaker = ~wavemaker;
				else running = 0;
				break;
			default:
				break;
    }

		if(wavemaker) wavemaker_refresh();
		else render_default_refresh();
    refresh();
  }

	play_stop();

  endwin();

  return 0;
}

