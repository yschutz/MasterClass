MCLibrary.so:	MCMain.cxx MCExercise.cxx MCExerciseJ1.cxx MCExerciseR1.cxx MCExerciseR2.cxx MCExerciseS1.cxx MCExerciseS2.cxx MCMultiView.cxx \
				MCVSDReader.cxx MCAliCalculator.cxx MCAliCounter.cxx MCAliSelector.cxx MCAliExtractor.cxx MCMultiLingual.cxx dict.cxx
	            g++ -g -shared -o $@ `root-config --cflags --libs` -lGui -lEve -lRGL -fPIC $^

dict.cxx:	MCMain.h MCExercise.h MCExerciseJ1.h MCExerciseR1.h MCExerciseR2.h MCExerciseS1.h MCExerciseS2.h MCMultiView.h  MCVSDReader.h MCAliCalculator.h \
            MCAliCounter.h MCAliSelector.h MCAliExtractor.h MCMultiLingual.h LinkDef.h
	        rootcling -f $@ -rml MCLibrary.so -p $^
clean:      MCLibrary.so  MCLibrary.so.dSYM dict_rdict.pcm dict.cxx
			rm -fr $^