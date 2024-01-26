include <../../AEPanelMaker/AEPanelMaker/panel.scad>

title = "random";
title_x = 6;
title_y = 76 ;
title_font_size = 5;
title_font = "Liberation Sans:style=bold";
title_rotate = 90;

units = 1;
inputs = 2 ;
outputs = 8;

pots_rv09 = [
    [3, 13, "PROB A"],
    [3, 3, "PROB B"]
];

leds = [
    [5, 36],
    [5, 33],
];

generate_panel();