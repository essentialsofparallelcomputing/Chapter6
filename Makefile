All: autovec autovec_function timestep mass_sum stencil GlobalSumsVectorized

autovec: autovec/build/stream_triad

autovec/build/stream_triad:
	cd autovec; mkdir build; cd build; cmake ..; make; ./stream_triad

autovec_function: autovec_function/build/stream_triad

autovec_function/build/stream_triad:
	cd autovec_function; mkdir build; cd build; cmake ..; make; ./stream_triad

timestep: timestep/build/timestep

timestep/build/timestep:
	cd timestep; mkdir build; cd build; cmake ..; make; ./timestep; ./timestep_opt; ./timestep_opt1; ./timestep_opt2; ./timestep_opt3

mass_sum: mass_sum/build/mass_sum

mass_sum/build/mass_sum:
	cd mass_sum; mkdir build; cd build; cmake ..; make; ./mass_sum

stencil: stencil/build/stencil

stencil/build/stencil:
	cd stencil; mkdir build; cd build; cmake ..; make; ./stencil; ./stencil_opt1; ./stencil_opt2

GlobalSumsVectorized: GlobalSumsVectorized/build/globalsums

GlobalSumsVectorized/build/globalsums:
	cd GlobalSumsVectorized; mkdir build; cd build; cmake ..; make; ./globalsums

clean:
	cd autovec; rm -rf build
	cd autovec_function; rm -rf build
	cd timestep; rm -rf build
	cd mass_sum; rm -rf build
	cd stencil; rm -rf build
	cd GlobalSumsVectorized; rm -rf build
