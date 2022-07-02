default: build

build:
	mkdir dist; cd dist; mkdir node; mkdir browser; cd ..;\

	mkdir build; cd build; \
	emcmake cmake -DBROWSER_VERSION=ON ..; make; cd ..; \
	mv build/MidifilePerformer.* dist/browser; \
	rm -rf build; \
	
	mkdir build; cd build; \
	emcmake cmake ..; make; cd ..; \
	mv build/MidifilePerformer.* dist/node; \
	rm -rf build; \

dist:
	if [ ! -d dist ]; \
	then { echo 1>&2 "no dist directory, run build first"; exit 1; }; \
	fi; \
	cp -f README.md; \
	cp -f LICENSE; \
	cp -f package.json ./dist; \
	sed -i '' 's#"main": "dist/node/MidifilePerformer.js"#"main": "node/MidifilePerformer.js"#' dist/package.json; \
	sed -i '' 's#"browser": "dist/browser/MidifilePerformer.js"#"browser": "browser/MidifilePerformer.js"#' dist/package.json; \

clean:
	if [ -d dist ]; then rm -rf dist; fi;

.PHONY: build dist clean