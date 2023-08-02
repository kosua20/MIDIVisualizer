#ifndef SR_GUI_INTERNAL_H
#define SR_GUI_INTERNAL_H

#ifndef SR_GUI_MALLOC
#	define SR_GUI_MALLOC(S) (malloc(S))
#endif

#ifndef SR_GUI_FREE
#	define SR_GUI_FREE(S) (free(S))
#endif

#ifndef SR_GUI_MEMCPY
#	include <stdlib.h>
#	define SR_GUI_MEMCPY(D, S, T) (memcpy(D, S, T))
#endif

#ifndef SR_GUI_MAX_STR_SIZE
#	define SR_GUI_MAX_STR_SIZE 1024
#endif

#define SR_GUI_APP_ID "com.sr.midivisualizer"
#define SR_GUI_APP_NAME "MIDIVisualizer"

#endif
