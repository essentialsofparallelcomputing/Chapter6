All: autovec autovec_function timestep mass_sum stencil GlobalSumsVectorized

autovec: autovec/stream_triad

autovec/stream_triad:
	cd autovec; cmake .; make; ./stream_triad

autovec_function: autovec_function/stream_triad

autovec_function/stream_triad:
	cd autovec_function; cmake .; make; ./stream_triad

timestep: timestep/timestep

timestep/timestep:
	cd timestep; cmake .; make; ./timestep; ./timestep_opt; ./timestep_opt1; ./timestep_opt2; ./timestep_opt3

mass_sum: mass_sum/mass_sum

mass_sum/mass_sum:
	cd mass_sum; cmake .; make; ./mass_sum

#stencil: stencil/stencil
#
#stencil/stencil:
#	cd stencil; cmake .; make; ./stencil; ./stencil_opt1; ./stencil_opt2

GlobalSumsVectorized: GlobalSumsVectorized/globalsums

GlobalSumsVectorized/globalsums:
	cd GlobalSumsVectorized; cmake .; make; ./globalsums

clean:
	cd autovec; make clean; make distclean
	cd autovec_function; make clean; make distclean
	cd timestep; make clean; make distclean
	cd mass_sum; make clean; make distclean
	#cd stencil; make clean; make distclean
	cd GlobalSumsVectorized; make clean; make distclean
