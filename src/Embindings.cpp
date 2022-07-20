#include <emscripten/bind.h>
#include "libMidifilePerformer/src/include/impl/MFPRenderer.h"

using namespace emscripten;

EMSCRIPTEN_BINDINGS(MidifilePerformer) {
  value_object<noteData>("noteData")
    .field("on",      &noteData::on)
    .field("pitch",   &noteData::pitch)
    .field("velocity",&noteData::velocity)
    .field("channel", &noteData::channel)
    ;

  value_object<commandData>("commandData")
    .field("pressed", &commandData::pressed)
    .field("id",      &commandData::id)
    .field("velocity",&commandData::velocity)
    .field("channel", &commandData::channel)
    ;

  register_vector<noteData>("noteDataSet");

  typedef ChronologyParams::parameters ChronologyParameters;

  value_object<ChronologyParameters>("chronologyParameters")
    .field("unmeet",              &ChronologyParameters::unmeet)
    .field("complete",            &ChronologyParameters::complete)
    .field("shift",               &ChronologyParameters::shift)
    .field("temporalResolution",  &ChronologyParameters::temporalResolution)
    ;

  class_<Chronology<noteData>>("Chronology")
    .constructor()
    .function("pushEvent",    &Chronology<noteData>::pushEvent)
    .function("finalize",     &Chronology<noteData>::finalize)
    .function("hasEvents",    &Chronology<noteData>::hasEvents)
    .function("pullEvents",   &Chronology<noteData>::pullEvents)
    .function("clear",        &Chronology<noteData>::clear)
    .function("size",         &Chronology<noteData>::size)
    ;

  typedef ChordVelocityMapping::StrategyType ChordStrategy;

  enum_<ChordStrategy>("chordStrategy")
    .value("sameForAll",              ChordStrategy::SameForAll)
    .value("clippedScaledFromMean",   ChordStrategy::ClippedScaledFromMean)
    .value("adjustedScaledFromMean",  ChordStrategy::AdjustedScaledFromMean)
    .value("clippedScaledFromMax",    ChordStrategy::ClippedScaledFromMax)
    ;

  typedef MFPRenderer Renderer;
  // typedef MFPRenderer<noteData, commandData, commandKey> Renderer;
  // typedef Renderer<noteData, commandData, commandKey> Renderer;

  class_<Renderer>("Renderer")
    .constructor<ChronologyParameters>()
    .function("setChordRenderingStrategy",&Renderer::setChordRenderingStrategy)
    .function("pushEvent",                &Renderer::pushEvent)
    .function("finalize",                 &Renderer::finalize)
    .function("hasEvents",                &Renderer::hasEvents)
    .function("pullEvents",               &Renderer::pullEvents)
    .function("pullEventsSet",            &Renderer::pullEventsSet)
    .function("combine3",                 &Renderer::combine3)
    // .function("combine3",
    //   select_overload<std::vector<noteData>(commandData, bool)>(&Renderer::combine3))
    .function("clear",                    &Renderer::clear)
    .function("setPartition",             &Renderer::setPartition)
    .function("getPartition",             &Renderer::getPartition)
    ;
}