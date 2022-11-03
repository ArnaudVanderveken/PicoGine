#pragma once

#define UNICODE

#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#define WIDE2(x) L##x
#define WIDE1(x) WIDE2(x)
#define __WFILE__ WIDE1(__FILE__)

#include <iostream>
#include <memory>
#include <sstream>
#include <wrl.h> //ComPtr