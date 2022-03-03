# Flow Field

A Flow Field creates for every cell in a Tilemap a direction to a target. Every enemy who than needs to go to the target can read the direction from the cell and does not need to calculate the path itself. This allows hundreds of enemies to pathfind simultaneously.
The plugin is currently only functional for windows. Other platforms are planned and the documentation will be updated.


## Installation
Flow Field is a plugin for Godot. You can follow the directions from the [Godot Documentation](https://docs.godotengine.org/de/stable/tutorials/plugins/editor/installing_plugins.html) or from this section.

### Installing plugin
Download the zip and extract the folder. Move the addons/ folder into your project. If you already have an addons folder merge both addons folder. If your file manager asks if you want to write into the folder answer **Yes**. No files will be overwritten.

### Activating plugin
Open **Project > Project Settings** at the top of the editor and go to the **Plugins** tab. There you should see the plugin **Flow Field**. Check the chekbox to activate the plugin.

### Creating a Flow Field
Flow Field is a TileMap and can be added over the add Node menu. If you already have a tilemap you wanna keep using, add the script **addons -> FlowField -> FlowField.gd** to the tilemap. If the node already has a script, change extend TileMap to extend FlowField. You might need to check with the Flow Field script if you are overwriting some of its functions.

Once you have the Flow Field in your scene you can set the cost of every tile type. For this go to your tilemap under the section Tile Costs. There a list of every tilename in the tileset with its cost is shown. The cost is initialized as one but can be changed from 0 to 254. 0 counts here as an untraversable tile.
When the costs are set you can create a Flow Field through code. With the function *create_flow_field(Vector2 target_cell)* a Flow Field is created where every cell has a direction that points to the next cell to reach the target_cell as fast as possible. If you change the TileMap during runtime you need to call the function *update_play_field()* before you create the Flow Field or else the Flow Field will be based on an outdated map. The function is not yet optimized so only call it if you changed the TileMap.
When the Flow Field is created you can get the direction to the next cell towards the target with *get_cell_direction(Vector2 cell)*.

## Member Variables

boolean [allow_diagonals] (#allow_diagonals)
boolean [diagonals_need_neighbors] (#diagonals_need_neighbors)
Dictionary [tiles] (#tiles)

## Member Variable Description

### allow_diagonals
If true the Flow Field allows movement in all eight directions instead of the basic 4 (North, South, West, East)

### diagonals_need_neighbors
If true diagonal movement is only possible if both direct neighbors are traversable e.g. if the direction is South-East then both the North and South neighbor need to be traversable

### tiles
Contains the cost of every tile type. Each tile can be accessed over tiles[tile_id]. They contain their name and the cost. If you want to change the cost of a tile with code you could do this with tiles[tile_id].cost = new_cost. New_cost has to be an integer and has to bee in the range 0-254 whereas 0=non traversable



## Member Functions
void [create_flow_field(Vector2 target_cell Array important_cells=[])] (#create_flow_fieldvector2-target_cell-array-important_cells=[]))

void [create_flow_fieldv(Array target_cells Array important_cells=[])] (#create_flow_fieldvarray-target_cells-array-important_cells=[]))

Vector2 [get_cell_direction(Vector2 cell)](#get_cell_directionvector2-cell))

int [get_cell_distance(Vector2 cell)](#get_cell_distancevector2-cell))

void [update_play_field()] (#update_play_field())


## Member Function Description

### create_flow_field(Vector2 target_cell Array important_cells=[])
Creates the Flow Field where every cell points towards target_cell. If every tile contained in important_cells got updated, the flow_field stops calculating. If there are no important cells all reachable cells from the target_cell will be calculated.

### create_flow_fieldv(Array target_cells Array important_cells=[]
Creates the Flow Field where every cell points towards the closest target_cell in target_cells.If every tile contained in important_cells got updated, the flow_field stops calculating. If there are no important cells all reachable cells from the target_cell will be calculated.

### Vector2 get_cell_direction(Vector2 cell)
Gives the direction of the Flow Field in the cell. returns zero vector if the cell is the target cell or there is no way to the target

### get_cell_distance(Vector2 cell)
Returns the distance from the cell to the next target_cell. The distance includes the cost of the tiles e.g. if the target cell is 3 Tiles with 5 cost each from the target_cell away then this function will return 15

### update_play_field()
Updates the representation of the TileMap in the Flow Field. If you change the TileMap you need to call this function before you create a new Flow Field or else the Flow Field will be based on the non updated TileMap. This function is not yet optimized and should only be called after the TileMap was changed and a new Flow Field will be created

