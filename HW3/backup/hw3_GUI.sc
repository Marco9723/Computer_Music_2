//-----------------------------------------------------------------------------------------------------------
Server.default = s = Server.internal.boot;
NetAddr("127.0.0.1",57120);

(SynthDef(\prova, {
	var sig, freq = 60, mode=40, prevMode = 41;
	var chords;

		sig = Mix.fill(5, {
			arg i;
			var temp, add;
			temp = Select.kr(15, chords);
			add = Select.kr(i, temp);
			SinOsc.ar((freq+add).midicps, 0, 0.2);
	    });

	if(mode > prevMode)
	{
		a = 0;
	}
	{
		a = 1;
	};
	Out.ar([0,1], sig);
}).play;
)

(
~index = 0;
 SynthDef(\osc,
	{
		arg freq = 40, amp = 0.2, freqfilter = 5000, t_trig = 1, chordIndex = 0, start = 0.5, att = 0.4, dec = 1.0,sus = 0.4, rel=1, slideDur = 0.8);
		var sig, src, env, prev = [40,40,40,40,40];
		var chords = [[0,3,9,11,26],
			[0,9,15,23,24],
			[0,9,15,23,26],
			[0,7,15,21,23],
			[0,7,15,21,26],
			[0,7,15,23,26],
			[0,10,15,19,26],
			[0,5,10,15,22],
			[0,10,15,17,26],
			[0,10,14,15,19],
			[0,10,15,19,26],
			[0,7,15,22,26],
			[0,5,11,14,21],
			[0,5,11,16,21],
			[0,5,10,14,21],
			[0,5,10,14,19],
			[0,5,10,15,19],
			[0,5,10,15,20],
			[0,7,10,14,16],
			[0,10,16,19,21],
			[0,9,16,19,22],
			[0,10,16,19,26],
			[0,10,16,21,26],
			[0,7,16,22,26],
			[0,7,14,16,21],
			[0,9,16,19,26],
			[0,11,16,19,21],
			[0,11,16,21,26],
			[0,7,16,21,26],
			[0,7,16,23,26],
			[0,9,11,19,23],
			[0,7,11,21,26],
			[0,7,9,19,26],
			[0,7,14,21,23],
			[0,7,14,23,28],
			[0,7,14,21,28],
			[0,7,11,18,21],
			[0,7,14,18,21],
			[0,9,11,18,26],
			[0,7,18,23,26],
			[0,7,14,18,23],
			[0,7,18,26,33]];

		~index = 0;

		src = Mix.fill(5, {
			arg i;
			var temp, add, slide;

			env = EnvGen.ar(Env.linen(start, att, dec, sus, rel), t_trig);

			temp = Select.kr(chordIndex, chords);
			add = Select.kr(i, temp);

			slide = EnvGen.ar(Env.new([(prev[~index]).midicps, (freq+add).midicps], [slideDur]), t_trig);

			prev[~index] = freq+add;
			~index = ~index +1;
			SinOsc.ar(slide, 0, amp*env);
	    });

		sig = BLowPass.ar(src,freq:freqfilter, rq:1);
		Out.ar([0,1], sig);
}).add;
)


(
~prevMode = 0;
~prevNoteIndex = 0;

a = Synth(\osc);

OSCdef('OSCreceiver',
	{
		arg msg, msgGUI;
		var noteIndex = 0, freqCutoff = 5000, mode = 0, range = 15, chordRange = 42, note = 40, startNote = 40;
		var notes = Array.series(range, start: startNote, step: 1);

		//noteIndex = round(msg[1]*(range-1)); //hue
		freqCutoff = msg[2]; //sat
		mode = round((1 / (1 + exp(-5*(msg[3] - 0.5)))) *(chordRange-1));
		noteIndex = 10.rand;

		if(mode > ~prevMode)
		{
			if((~prevNoteIndex+noteIndex) > (range-1))
			{
				note = notes[~prevNoteIndex];
			}
			{
				note = notes[~prevNoteIndex+noteIndex];
			};
		}
		{
			if(mode == ~prevMode)
			{
				note = notes[~prevNoteIndex];
			}
			{
				if((~prevNoteIndex-noteIndex) < 2)
				{
					note = notes[~prevNoteIndex];
				}
				{
					note = notes[~prevNoteIndex-noteIndex];
				};
			};
		};

		postln("noteIndex: "+noteIndex+"note: "+note+" - freqCutoff: "+((freqCutoff*2000)+20)+" mode :"+mode);

		~prevNoteIndex = note-startNote;
		~prevMode = mode;
		a.set(\freq, note, \freqfilter, msg[8], \chordIndex, mode, \t_trig, 1,\start = msg[3], \att = msg[4], \dec = msg[5],\sus = msg[6], \rel=msg[7], \slideDur = msg[9]);
}, "/color");
)

/*
# da chiaro a scuro
Matrix.with([[0,7,14,21,28], [0,5,10,15,20], [0,7,16,23,26], [0,7,16,22,26], [0,7,15,22,26], [0,7,11,15,21]]).postln

[[0,3,9,11,26],
[0,9,15,23,24],
[0,9,15,23,26],
[0,7,15,21,23],
[0,7,15,21,26],
[0,7,15,23,26],
[0,10,15,19,26],
[0,5,10,15,22],
[0,10,15,17,26],
[0,10,14,15,19],
[0,10,15,19,26],
[0,7,15,22,26],
[0,5,11,14,21],
[0,5,11,16,21],
[0,5,10,14,21],
[0,5,10,14,19],
[0,5,10,15,19],
[0,5,10,15,20],
[0,7,10,14,16],
[0,10,16,19,21],
[0,9,16,19,22],
[0,10,16,19,26],
[0,10,16,21,26],
[0,7,16,22,26],
[0,7,14,16,21],
[0,9,16,19,26],
[0,11,16,19,21],
[0,11,16,21,26],
[0,7,16,21,26],
[0,7,16,23,26],
[0,9,11,19,23],
[0,7,11,21,26],
[0,7,9,19,26],
[0,7,14,21,23],
[0,7,14,23,28],
[0,7,14,21,28],
[0,7,11,18,21],
[0,7,14,18,21],
[0,9,11,18,26],
[0,7,18,23,26],
[0,7,14,18,23],
[0,7,18,26,33]]


if(mode.asInteger > prevMode.asInteger)
		{
			if((prevNoteIndex+noteIndex) > (range-1))
			{
				note = Select.kr(prevNoteIndex, notes);
			}

			{
				note = Select.kr(prevNoteIndex+noteIndex, notes);
			};
		}
		{
			if((prevNoteIndex-noteIndex) < 0)
			{
				note = Select.kr(prevNoteIndex, notes);
			}

			{
				note = Select.kr(prevNoteIndex-noteIndex, notes);
			};
		};
*/