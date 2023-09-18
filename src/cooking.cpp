#include "player.h"
#include "cooking.h"
#include "messages.h"
#include "ui_manager.h"
#include <input.h>
#include "output.h"
using namespace catacurses;


void cooking_ui() {
    // Needed for UI to draw
    ui_adaptor ui;

    // Ncurses window for drawing
    catacurses::window window;
    // Takes care of the resizing procress
    ui.on_screen_resize([&](ui_adaptor& ui) {
    // Create an ncurses window
    // TERMX and TERMY are defined in output.h as the maximum terminal size
    window = catacurses::newwin(TERMX - 5, TERMY / 2, point(15,5));



    // The window passed to this call must contain all the space the redraw
    // callback draws to, to ensure proper refreshing when resizing or exiting
    // from other UIs.
    ui.position_from_window(window);
         });
    // Mark the resize callback to be called on the first redraw
    ui.mark_resize();
    // Things to do when redrawing the UI
    ui.on_redraw([&](const ui_adaptor&) {
    // Clear UI area
    werase(window);

    // Print things
    mvwprintw(window, point_zero, "Cooking menu, work in progress, ask again in two to three months");
    // Draw boxes around the window
    //mvwputch(window, point(0, getmaxy(window) - 1), BORDER_COLOR, LINE_OXXO); // |^
    //mvwputch(window, point(0, getmaxy(window) - 1), BORDER_COLOR, LINE_OXOX); // |^
    mvwputch(window, point(0, 0), BORDER_COLOR, LINE_OXXO); // |^
    mvwputch(window, point(0, 0), BORDER_COLOR, LINE_OXOX); // |^
    // Write to frame buffer
    wnoutrefresh(window);
    });
    add_msg("cooking_ui text");
    
    // Vodoo stuff to avoid the window for instat closing
    input_context ctxt("<CATEGORY_NAME>");
    ctxt.register_action("QUIT");
    while (true) {
        add_msg("context loop");
        // Invalidate the top UI (that is, this UI) and redraw all
        // invalidated UIs (including lower UIs that calls this UI).
        // May call the resize callbacks.
        ui_manager::redraw();
        // Get user input. Note that this may call the resize and redraw
        // callbacks multiple times due to screen resize, rendering target
        // reset, etc.
        const std::string action = ctxt.handle_input();
        if (action == "QUIT") {
            break;
            
        }
        
    }
}

void player::cook() {
    cooking_ui();
    add_msg("<insert cook window>");
}
