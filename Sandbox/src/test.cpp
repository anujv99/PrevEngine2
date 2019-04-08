
#include "prev.h"

#include "application.h"
#include "engine/window.h"

using namespace prev;

class Testapp : public Application {

};

Application * CreateApplication() {
	return new Testapp();
}
