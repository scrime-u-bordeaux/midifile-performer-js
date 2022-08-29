#include <emscripten/bind.h>
#include "libMidifilePerformer/src/include/impl/SequencePerformer.h"

using namespace emscripten;

// int64_t is not supported by emscripten so we need these utilities

// correct found here :
// https://github.com/emscripten-core/emscripten/issues/6492
// getters / setters can also be specified with optional_override(lambda), see :
// https://intrepidgeeks.com/tutorial/wasm-emcriptenembind-int64nknown-type-x

template <typename T>
double eventset_getdt(const Events::Set<T>& s) {
  return static_cast<double>(s.dt);
};

template <typename T>
void eventset_setdt(Events::Set<T>& s, double dt) {
  s.dt = static_cast<std::int64_t>(dt);
};

// specializations :

static double noteset_getdt(const Events::Set<noteData>& s) {
  return eventset_getdt<noteData>(s);
}

static void noteset_setdt(Events::Set<noteData>& s, double dt) {
  return eventset_setdt<noteData>(s, dt);
}

static double commandset_getdt(const Events::Set<commandData>& s) {
  return eventset_getdt<commandData>(s);
}

static void commandset_setdt(Events::Set<commandData>& s, double dt) {
  return eventset_setdt<commandData>(s, dt);
}

EMSCRIPTEN_BINDINGS(MidifilePerformer) {

  // NOTE STRUCTS //////////////////////////////////////////////////////////////

  value_object<noteData>("noteData")
    .field("on",      &noteData::on)
    .field("pitch",   &noteData::pitch)
    .field("velocity",&noteData::velocity)
    .field("channel", &noteData::channel)
    ;

  register_vector<noteData>("noteVector");

  value_object<Events::Set<noteData>>("noteSet")
    .field("dt",      &noteset_getdt, &noteset_setdt)
    .field("events",  &Events::Set<noteData>::events)
    ;

  value_object<Events::SetPair<noteData>>("noteSetPair")
    .field("start",   &Events::SetPair<noteData>::start)
    .field("end",     &Events::SetPair<noteData>::end)
    ;

  // COMMAND STRUCTS ///////////////////////////////////////////////////////////

  value_object<commandData>("commandData")
    .field("pressed", &commandData::pressed)
    .field("id",      &commandData::id)
    .field("velocity",&commandData::velocity)
    .field("channel", &commandData::channel)
    ;

  register_vector<commandData>("commandVector");

  value_object<Events::Set<commandData>>("commandSet")
    .field("dt",      &commandset_getdt, &commandset_setdt)
    .field("events",  &Events::Set<commandData>::events)
    ;

  value_object<Events::SetPair<commandData>>("commandSetPair")
    .field("start",   &Events::SetPair<commandData>::start)
    .field("end",     &Events::SetPair<commandData>::end)
    ;

  // CHRONOLOGY OPTIONS AND PARAMETERS /////////////////////////////////////////

  typedef Events::correspondOption ShiftMode;

  enum_<ShiftMode>("shiftMode")
    .value("pitchAndChannel", ShiftMode::PITCH_AND_CHANNEL)
    .value("pitchOnly",       ShiftMode::PITCH_ONLY)
    .value("none",            ShiftMode::NONE)
    ;

  typedef ChronologyParams::parameters ChronologyParameters;

  value_object<ChronologyParameters>("chronologyParameters")
    .field("unmeet",              &ChronologyParameters::unmeet)
    .field("complete",            &ChronologyParameters::complete)
    .field("shiftMode",           &ChronologyParameters::shiftMode)
    .field("temporalResolution",  &ChronologyParameters::temporalResolution)
    ;

  // CHRONOLOGY ////////////////////////////////////////////////////////////////
 
  typedef Chronology<noteData, std::vector> Chronology;

  class_<Chronology>("Chronology")
    .constructor()
    .function("size",       &Chronology::size)
    .function("clear",      &Chronology::clear)
    .function("pushEvent",  &Chronology::pushEvent)
    .function("finalize",   &Chronology::finalize)
    ;

  // PERFORMER STRATEGIES //////////////////////////////////////////////////////

  typedef ChordRendering::StrategyType ChordStrategy;

  enum_<ChordStrategy>("chordStrategy")
    .value("none",                    ChordStrategy::None)
    .value("sameForAll",              ChordStrategy::SameForAll)
    .value("clippedScaledFromMean",   ChordStrategy::ClippedScaledFromMean)
    .value("adjustedScaledFromMean",  ChordStrategy::AdjustedScaledFromMean)
    .value("clippedScaledFromMax",    ChordStrategy::ClippedScaledFromMax)
    ;

  typedef VoiceStealing::StrategyType VoiceStealingStrategy;

  enum_<VoiceStealingStrategy>("voiceStealingStrategy")
    .value("none",            VoiceStealingStrategy::None)
    .value("lastNoteOffWins", VoiceStealingStrategy::LastNoteOffWins)
    .value("onlyStaccato",    VoiceStealingStrategy::OnlyStaccato)
    ;

  // PERFORMER /////////////////////////////////////////////////////////////////

  typedef SequencePerformer Performer;

  class_<AbstractPerformer>("AbstractPerformer")
    .function(
      "setVoiceStealingStrategy",
      &AbstractPerformer::setVoiceStealingStrategy
    )
    .function(
      "setChordRenderingStrategy",
      &AbstractPerformer::setChordRenderingStrategy
    )
    ;
  class_<Performer, base<AbstractPerformer>>("Performer")
    .constructor<ChronologyParameters>()
    .function("size",                     &Performer::size)
    .function("clear",                    &Performer::clear)
    .function("pushEvent",                &Performer::pushEvent)
    .function("finalize",                 &Performer::finalize)
    .function("render",
      select_overload<std::vector<noteData>(commandData)>(&Performer::render))
    .function("setChronology",            &Performer::setChronology)
    .function("getChronology",            &Performer::getChronology)
    ;
}