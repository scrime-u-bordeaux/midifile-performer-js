default: build

build:
	emcmake cmake -B build/browser -DBROWSER_VERSION=ON; \
  cmake --build build/browser; \
	
	emcmake cmake -B build/node -DNODE_VERSION=ON; \
  cmake --build build/node; \

dist:
	if [ ! -d build ]; \
	then { echo 1>&2 "no build directory, run \"npm run build\" first"; exit 1; }; \
	fi; \

	mkdir dist; \

	mkdir dist/browser; \
	cp build/browser/MidifilePerformer.* dist/browser; \

	mkdir dist/node; \
	cp build/node/MidifilePerformer.* dist/node; \

	rm -rf build

clean:
	if [ -d build ]; then rm -rf build; fi; \
	if [ -d dist ]; then rm -rf dist; fi; \

.PHONY: build dist clean