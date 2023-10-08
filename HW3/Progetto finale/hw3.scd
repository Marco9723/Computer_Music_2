//-----------------------------------------------------------------------------------------------------------
NetAddr("127.0.0.1",57120);

(
~index = 0;
 SynthDef(\osc,
	{
		arg rootNote = 40, amp = 0.2, reverbMix = 0, trigEnv = 0, t_trigSlide = 0, chordIndex = 0,  freqfilter = 5000, peak = 0.5, att = 0.4, dec = 0.0,sus = 1, rel=0.5, slideDur = 0.8;
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

		//building chords
		src = Mix.fill(5, {
			arg i;
			var temp, add, slide;

			//retrieve the selected chord
			temp = Select.kr(chordIndex, chords);
			//the offset value of each note
			add = Select.kr(i, temp);

			slide = EnvGen.ar(
				Env.new(
					[(prev[~index]).midicps,
					(rootNote+add).midicps],
					[slideDur]),
				t_trigSlide);

			prev[~index] = rootNote+add;
			~index = ~index +1;
			LFSaw.ar(slide, 0, amp);
	    });

		sig = BLowPass.ar(src,freq:freqfilter, rq:1);
		sig = FreeVerb.ar(sig, reverbMix, 0.6, 0.8);

		env = EnvGen.ar(
			Env.adsr(att, dec, sus, rel, peak),
			trigEnv);

		Out.ar([0,1], sig*env);
}).add;
)


(
a = Synth(\osc);

OSCdef('OSCreceiver',
	{
		arg msg;
		var reverbMix = 0, chord = 0, note = 40;

		reverbMix = msg[3]; //sat
		note = msg[4];
		chord = msg[5];

		if(msg[2] == true, {
			a.set(\rootNote, note,
			\chordIndex, chord,
			\reverbMix, reverbMix,
			\trigEnv, 1,
			\t_trigSlide, 1,
			\peak, msg[6],
			\att, msg[7],
			\dec, msg[8],
			\sus, msg[9],
			\rel, msg[10],
			\freqfilter, msg[11],
			\slideDur, msg[12]);
		},{
			if(msg[1] == true, {
				a.set(\rootNote, note,
					\chordIndex, chord,
					\reverbMix, reverbMix,
					\t_trigSlide, 1,
					\trigEnv, 1);
			},{
				a.set(
					\trigEnv, 0);
			});
		});

}, "/color");
)