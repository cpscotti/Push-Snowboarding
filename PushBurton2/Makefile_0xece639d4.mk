# ==============================================================================
# Generated by qmake (2.01a) (Qt 4.6.3) on: ter 25. jan 17:58:01 2011
# This file is generated by qmake and should not be modified by the
# user.
#  Name        : Makefile_0xece639d4.mk
#  Part of     : PushBurton2
#  Description : This file is used to call necessary targets on wrapper makefile
#                during normal Symbian build process.
#  Version     : 
#
# ==============================================================================



MAKE = make

VISUAL_CFG = RELEASE
ifeq "$(CFG)" "UDEB"
VISUAL_CFG = DEBUG
endif

do_nothing :
	@rem do_nothing

MAKMAKE: create_temps pre_targetdeps store_build

LIB: create_temps pre_targetdeps store_build

BLD: create_temps pre_targetdeps store_build

ifeq "$(PLATFORM)" "WINSCW"
CLEAN: extension_clean winscw_deployment_clean deployment_clean
else
CLEAN: extension_clean deployment_clean
endif

CLEANLIB: do_nothing

RESOURCE: do_nothing

FREEZE: do_nothing

SAVESPACE: do_nothing

RELEASABLES: do_nothing

ifeq "$(PLATFORM)" "WINSCW"
FINAL: finalize winscw_deployment deployment
else
FINAL: finalize deployment
endif

pre_targetdeps : c:\pushsnowboarding\PushBurton2\Makefile
	-$(MAKE) -f "c:\pushsnowboarding\PushBurton2\Makefile" pre_targetdeps QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

create_temps : c:\pushsnowboarding\PushBurton2\Makefile
	-$(MAKE) -f "c:\pushsnowboarding\PushBurton2\Makefile" create_temps QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

extension_clean : c:\pushsnowboarding\PushBurton2\Makefile
	-$(MAKE) -f "c:\pushsnowboarding\PushBurton2\Makefile" extension_clean QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

finalize : c:\pushsnowboarding\PushBurton2\Makefile
	-$(MAKE) -f "c:\pushsnowboarding\PushBurton2\Makefile" finalize QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

winscw_deployment_clean : c:\pushsnowboarding\PushBurton2\Makefile
	-$(MAKE) -f "c:\pushsnowboarding\PushBurton2\Makefile" winscw_deployment_clean QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

winscw_deployment : c:\pushsnowboarding\PushBurton2\Makefile
	-$(MAKE) -f "c:\pushsnowboarding\PushBurton2\Makefile" winscw_deployment QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

deployment_clean : c:\pushsnowboarding\PushBurton2\Makefile
	-$(MAKE) -f "c:\pushsnowboarding\PushBurton2\Makefile" deployment_clean QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

deployment : c:\pushsnowboarding\PushBurton2\Makefile
	-$(MAKE) -f "c:\pushsnowboarding\PushBurton2\Makefile" deployment QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)

store_build : c:\pushsnowboarding\PushBurton2\Makefile
	-$(MAKE) -f "c:\pushsnowboarding\PushBurton2\Makefile" store_build QT_SIS_TARGET=$(VISUAL_CFG)-$(PLATFORM)


