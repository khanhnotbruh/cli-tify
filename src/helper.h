#ifndef ENUM_H
#define ENUM_H
enum side_idx_e{
  TOP=0,
  BOTTOM,
  LEFT,
  RIGHT,
  SIDE_COUNT,
};
enum corner_idx_e{
  TOP_LEFT=0,
  TOP_RIGHT,
  BOTTOM_LEFT,
  BOTTOM_RIGHT,
  CORNER_COUNT,
};
enum edge_idx_e{
  EDGE_PATTERN=0,
  EDGE_MIDDLE,
  EDGE_UPPER,
  EDGE_LOWER,
  EDGE_RIGHT,
  EDGE_LEFT,
  EDGE_COUNT,
};
enum border_idx_e{
  EDGES=0,
  CORNERS,
  PATTERN,
  BORDER_COUNT,
};
enum state_idx_e{
  CX,
  CY,
  KEY_PRESSED,
  IS_HOVERED,   
  IS_EMPTY,
  IS_FOCUS,
  IS_VISIBLE, 
  IS_CLICKED,   
  IS_PRESSED,
  STATE_COUNT,
};
enum config_idx_e{
  ID=0,
  ANCHORS,
  W,
  H,
  SX,
  SY,
  ASCII,     
  EMPTY,   
  FOCUS,      
  MOUSE,      
  FALLTHROUGH,
  DRAG,       
  PRESERVE_LAYERS,
  FILL,
  ON_EVENT,
  TEXT,
  CONFIG_COUNT,
};
enum memory_component_type_e{
  WIDGET=0,
  BORDER,
  STATE,
  CONFIG,
  MEM_TYPE_COUNT,
};
enum memory_pointer_type_e{
  ROOT=0,
  CURR=1,
};
// id, name, ltype, ctype, macro
#endif
