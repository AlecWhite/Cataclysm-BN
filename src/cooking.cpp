#include "player.h"
#include "cooking.h"
#include "messages.h"
#include "ui_manager.h"
#include <input.h>
#include "output.h"
using namespace catacurses;

bool isWide = false;
int windowHeight = 5;
int windowWidth;
std::string type_of_dish = "none";













void cooking_ui() {
    // Needed for UI to draw
    ui_adaptor ui;

    // Ncurses window for drawing
    catacurses::window head_window;
    catacurses::window item_list_window;
    catacurses::window recipe_name_window;
    catacurses::window ingredient_list_window;
    catacurses::window stats_window;
    // Takes care of the resizing procress
    ui.on_screen_resize([&](ui_adaptor& ui) {
    
        //Cargo culting from crafting_gui to get the the correct width
        const int freeWidth = TERMX - FULL_SCREEN_WIDTH;
        isWide = (TERMX > FULL_SCREEN_WIDTH && freeWidth > 15);
        windowWidth = isWide ? (freeWidth > FULL_SCREEN_WIDTH ? FULL_SCREEN_WIDTH * 2 : TERMX) :
            FULL_SCREEN_WIDTH;
        int window_start_point = (TERMX - windowWidth) / 2;


        // Create an ncurses window
        // windowHeigth is 5
        // point(x, y) marks the position of the where the windows starts
        head_window = catacurses::newwin(windowHeight,windowWidth, point(window_start_point,0));
        item_list_window = catacurses::newwin(windowHeight, windowWidth / 2 ,point(window_start_point,5));
        recipe_name_window = catacurses::newwin(windowHeight, windowWidth / 2 , point(window_start_point + (window_start_point*2), 5));
        ingredient_list_window = catacurses::newwin(windowHeight, windowWidth / 2, point(window_start_point + (window_start_point * 2), 10));
        stats_window = catacurses::newwin(windowHeight, windowWidth / 2, point(window_start_point + (window_start_point * 2), 15));
       

        // The window passed to this call must contain all the space the redraw
        // callback draws to, to ensure proper refreshing when resizing or exiting
        // from other UIs.
        ui.position_from_window(head_window);
        ui.position_from_window(item_list_window);
        ui.position_from_window(recipe_name_window);
        ui.position_from_window(ingredient_list_window);
        ui.position_from_window(stats_window);
        fold_and_print(item_list_window, point(5,1),10, h_white,"Item list text la la la",1);
         });

    // Mark the resize callback to be called on the first redraw
    ui.mark_resize();
    // Things to do when redrawing the UI
    ui.on_redraw([&](const ui_adaptor&) {
    // Clear UI area
    werase(head_window);
    werase(item_list_window);
    werase(recipe_name_window);
    werase(ingredient_list_window);
    werase(stats_window);

    // Print things
    mvwprintw(head_window, point_zero, "Head window");
    mvwprintw(item_list_window, point_zero, "Item list window");
    mvwprintw(recipe_name_window, point_zero, "Recipe name window");
    mvwprintw(ingredient_list_window, point_zero, "Ingredient list window");
    mvwprintw(stats_window, point_zero, "Stats window");

    // Write to frame buffer
    wnoutrefresh(head_window);
    wnoutrefresh(item_list_window);
    wnoutrefresh(recipe_name_window);
    wnoutrefresh(ingredient_list_window);
    wnoutrefresh(stats_window);
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
