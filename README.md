# midifile-performer-js

This is an emscripted version of the [libMidifilePerformer](https://github.com/scrime-u-bordeaux/libMidifilePerformer) c++ library, based on the work of Jean Haury and Bernard P. Serpette.  
It provides a main entry point, the `Performer` class.  
A Performer must first be fed with a succession of MIDI note events extracted from a MIDI file.  
These notes will be reorganized into `Chronology` classes.  
Chronologies are degraded representations of the MIDI files that allow to reinterpret them with a few command keys in a very musically expressive way.

### Usage

* nodejs
```js
const MidifilePerformer = require('midifile-performer');

MidifilePerformer.onRuntimeInitialized = function() {
  const performer = new MidifilePerformer.Performer({
    unmeet:             true,
    complete:           false,
    shiftMode:          MidifilePerformer.shiftMode.pitchAndChannel,
    temporalResolution: 0,
  });

  performer.setChordVelocityMappingStrategy(
    MidifilePerformer.chordStrategy.sameForAll
  );
}
```

* browser
```js
import MidifilePerformer from 'midifile-performer';

async initialize() {
  this.mfp = await MidifilePerformer();

  this.performer = new this.mfp.Performer({
    unmeet: true,
    complete: false,
    shiftMode: this.mfp.shiftMode.pitchAndChannel,
    temporalResolution: chordDeltaMsDateThreshold,
  });
  this.performer.setChordVelocityMappingStrategy(
    this.mfp.chordStrategy.none,
    // this.mfp.chordStrategy.clippedScaledFromMax,
  );
  this.performer.setLooping(true);
}

initialize();

/* ... */
```

### Build instructions

requirements

* cmake
* emscripten (clone [emsdk](https://github.com/emscripten-core/emsdk) and follow installation procedure)

commands

* `npm run build`
* `npm run test`