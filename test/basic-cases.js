const test = require('tape');
const MidifilePerformer = require('../dist/node/MidifilePerformer.js');

/* * * * * * * * * * * * * * * * * UTILITIES * * * * * * * * * * * * * * * * */

const DEF_ON_VELOCITY = 127
const DEF_OFF_VELOCITY = 0;
const DEF_CHANNEL = 1;

function note(on, pitch, velocity = null, channel = DEF_CHANNEL) {
    if (velocity === null) {
        velocity = on ? DEF_ON_VELOCITY : DEF_OFF_VELOCITY;
    }

    return { on, pitch, velocity, channel };
}

function command(pressed, id, velocity = DEF_ON_VELOCITY, channel = DEF_CHANNEL) {
    return { pressed, id, velocity, channel };
}

////////// scores

const before = [
    [ 0, true,  60 ],
    [ 5, false, 60 ],
    [ 5, true,  62 ],
    [ 5, false, 62 ]
];

const meets = [
    [ 0, true,  60 ],
    [ 5, false, 60 ],
    [ 0, true,  62 ],
    [ 5, false, 62 ]
];

const overlaps = [
    [ 0, true,  60 ],
    [ 5, true,  62 ],
    [ 5, false, 60 ],
    [ 5, false, 62 ]
];

const starts = [
    [ 0, true,  60 ],
    [ 0, true,  62 ],
    [ 5, false, 60 ],
    [ 5, false, 62 ]
];

const during = [
    [ 0, true,  60 ],
    [ 5, true,  62 ],
    [ 5, false, 62 ],
    [ 5, false, 60 ]
];

const finishes = [
    [ 0, true,  60 ],
    [ 5, true,  62 ],
    [ 5, false, 60 ],
    [ 0, false, 62 ]
];

const equals = [
    [ 0, true,  60 ],
    [ 0, true,  62 ],
    [ 5, false, 60 ],
    [ 0, false, 62 ]
];

const normalScore = [
    [ 0, true, 60 ],
    [ 0, true, 64 ],
    [ 0, true, 67 ],
    [ 0, true, 68 ],
    [ 5, false, 67 ],
    [ 5, false, 64 ],
    [ 5, false, 60 ],
    [ 5, false, 68 ],
];

////////// commands

const beforeCmd = [
    [ true,  60 ],
    [ false, 60 ],
    [ true,  62 ],
    [ false, 62 ]
];

const overlapsCmd = [
    [ true,  60 ],
    [ true,  62 ],
    [ false, 60 ],
    [ false, 62 ]
];

const duringCmd = [
    [ true,  60 ],
    [ true,  62 ],
    [ false, 62 ],
    [ false, 60 ]
];

const longCommandSeries = [
    [ true, 60 ],
    [ false, 60 ],
    [ true, 62 ],
    [ false, 62 ],
];

function makeArray(array) {
    const out = [];
    for (let j = 0; j < array.size(); j++) {
        out.push(array.get(j));
    }
    return out;
}

function writeScoreToRenderer(renderer, notes) {
    renderer.clear();
    notes.forEach(n => {
        const [
            dt, on, pitch,
            velocity = on ? DEF_ON_VELOCITY : DEF_OFF_VELOCITY,
            channel = DEF_CHANNEL
        ] = n;
        renderer.pushEvent(dt, note(on, pitch, velocity, channel));
    });
    renderer.finalize();
}

function getRendererEventSets(renderer) {
    const out = [];
    while (renderer.hasEvents(true)) {
        const eventsSet = makeArray(renderer.pullEvents());
        out.push(eventsSet);
    }
    return out;
}

function getRendererOutput(renderer, commands) {
    const out = [];
    commands.forEach(c => {
        const [
            pressed, id,
            velocity = pressed ? DEF_ON_VELOCITY : DEF_OFF_VELOCITY,
            channel = DEF_CHANNEL
        ] = c;
        const eventSet = makeArray(
            renderer.combine3(command(pressed, id, velocity, channel), true)
        );
        out.push(eventSet);
    })
    return out;
}


/* * * * * * * * * * * * * * * * * * TESTS * * * * * * * * * * * * * * * * * */

MidifilePerformer.onRuntimeInitialized = function() {
    const renderer = new MidifilePerformer.Renderer({
        unmeet:             true,
        complete:           false,
        shiftMode:          MidifilePerformer.shiftMode.pitchAndChannel,
        temporalResolution: 0,
    });

    test('chord velocities', function(t) {
        t.plan(1);
        const score = [
            [ 0, true, 60, 127 ],
            [ 0, true, 64, 100 ],
            [ 0, true, 67, 4 ],
            [ 5, false, 60 ],
            [ 0, false, 64 ],
            [ 0, false, 67 ],
        ];
        function makeChordCommand(velocity) {
            return [
                [ true, 60, velocity ],
                [ false, 60],
            ];
        }

        renderer.setChordRenderingStrategy(
            //MidifilePerformer.chordStrategy.sameForAll
            //MidifilePerformer.chordStrategy.clippedScaledFromMean
            //MidifilePerformer.chordStrategy.adjustedScaledFromMean
            MidifilePerformer.chordStrategy.clippedScaledFromMax
        );

        let output;
        writeScoreToRenderer(renderer, score);
        const model = renderer.getPartition();
        
        output = getRendererOutput(renderer, makeChordCommand(127));
        console.log(output);

        renderer.setPartition(model);
        output = getRendererOutput(renderer, makeChordCommand(64));
        console.log(output);

        renderer.setPartition(model);
        output = getRendererOutput(renderer, makeChordCommand(1));
        console.log(output);

        t.equal(true, true);
    });

    test('overlap', function(t) {
        t.plan(1);
        // writeScoreToRenderer(renderer, normalScore);
        // const output = getRendererOutput(renderer, longCommandSeries);
        // const output = getRendererEventSets(renderer);
        // console.log(output);
        t.equal(true, true);
    });

    test('begins', function(t) {
        t.plan(1);
        t.equal(false, false);
    });
};
