//-----------------------------------------------------------------------------------------------------------
NetAddr("127.0.0.1",57120);

(
~index = 0;
 SynthDef(\osc,
	{
		arg freq = 40, amp = 0.2, reverbMix = 0, t_trig = 1, chordIndex = 0,  freqfilter = 5000, peak = 0.5, att = 0.4, dec = 1.0,sus = 0.4, rel=1, slideDur = 0.8;
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

			env = EnvGen.ar(Env.adsr(att, dec, sus, rel, peak), t_trig);

			temp = Select.kr(chordIndex, chords);
			add = Select.kr(i, temp);

			slide = EnvGen.ar(Env.new([(prev[~index]).midicps, (freq+add).midicps], [slideDur]), t_trig);

			prev[~index] = freq+add;
			~index = ~index +1;
			LFSaw.ar(slide, 0, amp*env*2);
	    });

		sig = BLowPass.ar(src,freq:freqfilter, rq:1);
		sig = FreeVerb.ar(sig, reverbMix, 0.5, 0.6);

		Out.ar([0,1], sig);
}).add;
)


(
a = Synth(\osc);

OSCdef('OSCreceiver',
	{
		arg msg;
		var reverbMix = 0, mode = 0, note = 40;

		reverbMix = msg[1]; //sat
		note = msg[2];
		mode = msg[3];

		postln("note: "+msg[2]+" - mode: "+msg[3]+" reverbMix :"+msg[1]);

		a.set(\freq, note,
			\chordIndex, mode,
			\reverbMix, reverbMix,
			\t_trig, 1,
			\peak, msg[4],
			\att, msg[5],
			\dec, msg[6],
			\sus, msg[7],
			\rel, msg[8],
			\freqfilter, msg[9],
			\slideDur, msg[10]
		);
}, "/color");
)