#ifndef EFL_GESTURE_PRIVATE_H_
#define EFL_GESTURE_PRIVATE_H_

#include "evas_common_private.h"
#include "evas_private.h"

#include "efl_canvas_gesture_events.eo.h"

#include <Ecore.h>

/* milliseconds */
#define TAP_TOUCH_TIME_THRESHOLD (0.1 * 1000)

const Efl_Event_Description * _efl_gesture_type_get(const Eo *obj);
int _direction_get(Evas_Coord xx1, Evas_Coord xx2);
Eina_Value *_recognizer_config_get(const Eo *obj, const char *name);
Eina_Bool _event_multi_touch_get(const Efl_Canvas_Gesture_Touch *event);

typedef struct _Efl_Canvas_Gesture_Manager_Data                Efl_Canvas_Gesture_Manager_Data;
typedef struct _Efl_Canvas_Gesture_Recognizer_Data             Efl_Canvas_Gesture_Recognizer_Data;
typedef struct _Efl_Canvas_Gesture_Recognizer_Tap_Data         Efl_Canvas_Gesture_Recognizer_Tap_Data;
typedef struct _Efl_Canvas_Gesture_Recognizer_Long_Tap_Data    Efl_Canvas_Gesture_Recognizer_Long_Tap_Data;
typedef struct _Efl_Canvas_Gesture_Recognizer_Double_Tap_Data  Efl_Canvas_Gesture_Recognizer_Double_Tap_Data;
typedef struct _Efl_Canvas_Gesture_Recognizer_Triple_Tap_Data  Efl_Canvas_Gesture_Recognizer_Triple_Tap_Data;
typedef struct _Efl_Canvas_Gesture_Recognizer_Momentum_Data    Efl_Canvas_Gesture_Recognizer_Momentum_Data;
typedef struct _Efl_Canvas_Gesture_Recognizer_Flick_Data       Efl_Canvas_Gesture_Recognizer_Flick_Data;
typedef struct _Efl_Canvas_Gesture_Recognizer_Zoom_Data        Efl_Canvas_Gesture_Recognizer_Zoom_Data;
typedef struct _Efl_Canvas_Gesture_Recognizer_Custom_Data        Efl_Canvas_Gesture_Recognizer_Custom_Data;
typedef struct _Efl_Canvas_Gesture_Data                        Efl_Canvas_Gesture_Data;
typedef struct _Efl_Canvas_Gesture_Momentum_Data               Efl_Canvas_Gesture_Momentum_Data;
typedef struct _Efl_Canvas_Gesture_Flick_Data                  Efl_Canvas_Gesture_Flick_Data;
typedef struct _Efl_Canvas_Gesture_Zoom_Data                   Efl_Canvas_Gesture_Zoom_Data;
typedef struct _Efl_Canvas_Gesture_Custom_Data                 Efl_Canvas_Gesture_Custom_Data;

typedef struct _Efl_Canvas_Gesture_Touch_Data
{
   Efl_Canvas_Gesture_Touch_State state;
   Eina_Array             *touch_points;
   Efl_Gesture_Touch_Point_Data *cur_touch;
   Efl_Gesture_Touch_Point_Data *prev_touch;
   Eo                     *target;
   int                     touch_down;
} Efl_Canvas_Gesture_Touch_Data;

struct _Efl_Canvas_Gesture_Recognizer_Data
{
   Eina_Bool                      continues;
};

struct _Efl_Canvas_Gesture_Recognizer_Tap_Data
{
   Eo                             *target;
   Eo                             *gesture;
   Ecore_Timer                    *timeout;
   int                            finger_size;
};

struct _Efl_Canvas_Gesture_Recognizer_Long_Tap_Data
{
   double                          start_timeout;
   Eina_List                      *target_timeout;
   Eo                             *target;
   Efl_Canvas_Gesture             *gesture;
   Ecore_Timer                    *timeout;
   int                            finger_size;
   Eina_Bool                       is_timeout;
};

struct _Efl_Canvas_Gesture_Recognizer_Double_Tap_Data
{
   double                          start_timeout;
   Eina_List                      *target_timeout;
   Eo                             *target;
   Eo                             *gesture;
   Ecore_Timer                    *timeout;
   int                             tap_count;
   int                            finger_size;
   Eina_Bool                       is_timeout;
};

struct _Efl_Canvas_Gesture_Recognizer_Triple_Tap_Data
{
   double                          start_timeout;
   Eina_List                      *target_timeout;
   Eo                             *target;
   Eo                             *gesture;
   Ecore_Timer                    *timeout;
   int                             tap_count;
   int                            finger_size;
   Eina_Bool                       is_timeout;
};

struct _Efl_Canvas_Gesture_Recognizer_Momentum_Data
{
   Eina_Position2D                 st_line;
   Eina_Position2D                 end_line;
   unsigned int                    t_st;
   unsigned int                    t_end;
   int                             xdir;
   int                             ydir;
   Eina_Bool                       touched;
};

struct _Efl_Canvas_Gesture_Recognizer_Flick_Data
{
   double                          line_angle;
   Eina_Position2D                 st_line;
   unsigned int                    t_st;
   unsigned int                    t_end;
   int                             line_length;
   int                            finger_size;
   Eina_Bool                       touched;
};

struct _Efl_Canvas_Gesture_Recognizer_Zoom_Data
{
   Efl_Gesture_Touch_Point_Data                    zoom_st;
   Efl_Gesture_Touch_Point_Data                    zoom_st1;

   Efl_Gesture_Touch_Point_Data                    zoom_mv;
   Efl_Gesture_Touch_Point_Data                    zoom_mv1;

   double                          zoom_distance_tolerance;
   double                          zoom_finger_factor;
   double                          zoom_step;
   double                          next_step;

   Evas_Coord                      zoom_base; /* Holds gap between fingers on
							                   * zoom-start  */
   int                            finger_size;
   Eina_Bool                       calc_temp;
};

struct _Efl_Canvas_Gesture_Recognizer_Custom_Data
{
   Eina_Stringshare *name;
};

struct _Efl_Canvas_Gesture_Data
{
   Efl_Canvas_Gesture_State        state;
   const Efl_Event_Description    *type;
   Eina_Position2D                 hotspot;
   unsigned int                    timestamp;
   unsigned int                    touch_count;
};

struct _Efl_Canvas_Gesture_Momentum_Data
{
   Eina_Vector2                    momentum;
   int id;
};

struct _Efl_Canvas_Gesture_Flick_Data
{
   Eina_Vector2                    momentum;
   double                          angle;
   int id;
};

struct _Efl_Canvas_Gesture_Zoom_Data
{
   double                          radius;
   double                          zoom;
};

struct _Efl_Canvas_Gesture_Custom_Data
{
   Eina_Stringshare *gesture_name;
};

#endif
