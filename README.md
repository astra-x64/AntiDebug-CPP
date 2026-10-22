# AntiDebug-CPP

1. Protection: PEB (BeingDebugged) Check
2. Protection : Hardware Breakpoint Control (GetThreadContext)
3. Thread Hiding (ThreadHideFromDebugger/NtSetInformationThread)

# Usage

- First, add the anti_debug.h header file to your project. Then, include it in your main.cpp file:

 #include "anti_debug.h"

- Finally, call the RunAll(); function inside your main function.
