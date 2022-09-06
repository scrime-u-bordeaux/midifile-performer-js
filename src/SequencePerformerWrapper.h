#ifndef MFP_SEQUENCEPERFORMERWRAPPER_h
#define MFP_SEQUENCEPERFORMERWRAPPER_h

#include <emscripten/bind.h>
#include "libMidifilePerformer/src/include/impl/SequencePerformer.h"

class SequencePerformerWrapper : public SequencePerformer {
public:
  SequencePerformerWrapper() :
  SequencePerformer() {}

  SequencePerformerWrapper(ChronologyParams::parameters params) :
  SequencePerformer(params) {}

  virtual void setNoteEventsCallback(emscripten::val lambda) {
    SequencePerformer::setNoteEventsCallback(
      [lambda](std::vector<noteData> notes) {
        if (!lambda.isNull()) {
          lambda(notes);
        }
      }
    );
  }
};

#endif /* MFP_SEQUENCEPERFORMERWRAPPER_h */