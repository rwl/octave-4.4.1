## DO NOT EDIT -- generated from module-files by config-module.awk

EXTRA_DIST += \
  %reldir%/config-module.sh \
  %reldir%/config-module.awk \
  %reldir%/module-files \
  %reldir%/oct-qhull.h

DLDFCN_SRC = \
  %reldir%/__delaunayn__.cc \
  %reldir%/__eigs__.cc \
  %reldir%/__fltk_uigetfile__.cc \
  %reldir%/__glpk__.cc \
  %reldir%/__init_fltk__.cc \
  %reldir%/__init_gnuplot__.cc \
  %reldir%/__ode15__.cc \
  %reldir%/__osmesa_print__.cc \
  %reldir%/__voronoi__.cc \
  %reldir%/amd.cc \
  %reldir%/audiodevinfo.cc \
  %reldir%/audioread.cc \
  %reldir%/ccolamd.cc \
  %reldir%/chol.cc \
  %reldir%/colamd.cc \
  %reldir%/convhulln.cc \
  %reldir%/dmperm.cc \
  %reldir%/fftw.cc \
  %reldir%/gzip.cc \
  %reldir%/qr.cc \
  %reldir%/symbfact.cc \
  %reldir%/symrcm.cc

DLDFCN_LIBS = $(DLDFCN_SRC:.cc=.la)

if AMCOND_ENABLE_DYNAMIC_LINKING

octlib_LTLIBRARIES += $(DLDFCN_LIBS)

## Use stamp files to avoid problems with checking timestamps
## of symbolic links

%.oct : %.la
	$(AM_V_GEN)$(INSTALL_PROGRAM) %reldir%/.libs/$(shell $(SED) -n -e "s/dlname='\([^']*\)'/\1/p" < $<) $@

else

noinst_LTLIBRARIES += $(DLDFCN_LIBS)

endif

%canon_reldir%___delaunayn___la_SOURCES = %reldir%/__delaunayn__.cc
%canon_reldir%___delaunayn___la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(QHULL_CPPFLAGS)
%canon_reldir%___delaunayn___la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(QHULL_CPPFLAGS)
%canon_reldir%___delaunayn___la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(QHULL_CPPFLAGS)
%canon_reldir%___delaunayn___la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(QHULL_LDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%___delaunayn___la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(QHULL_LIBS) $(OCT_LINK_DEPS)

%canon_reldir%___eigs___la_SOURCES = %reldir%/__eigs__.cc
%canon_reldir%___eigs___la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(ARPACK_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%___eigs___la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(ARPACK_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%___eigs___la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(ARPACK_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%___eigs___la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(ARPACK_LDFLAGS) $(SPARSE_XLDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%___eigs___la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(ARPACK_LIBS) $(SPARSE_XLIBS) $(LAPACK_LIBS) $(BLAS_LIBS) $(OCT_LINK_DEPS)

%canon_reldir%___fltk_uigetfile___la_SOURCES = %reldir%/__fltk_uigetfile__.cc
%canon_reldir%___fltk_uigetfile___la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(FLTK_CPPFLAGS) $(FT2_CPPFLAGS)
%canon_reldir%___fltk_uigetfile___la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(FLTK_CPPFLAGS) $(FT2_CPPFLAGS)
%canon_reldir%___fltk_uigetfile___la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(FLTK_CPPFLAGS) $(FT2_CPPFLAGS)
%canon_reldir%___fltk_uigetfile___la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(FLTK_LDFLAGS) $(FT2_LDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%___fltk_uigetfile___la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(FLTK_LIBS) $(FT2_LIBS) $(OCT_LINK_DEPS)

%canon_reldir%___glpk___la_SOURCES = %reldir%/__glpk__.cc
%canon_reldir%___glpk___la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(GLPK_CPPFLAGS)
%canon_reldir%___glpk___la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(GLPK_CPPFLAGS)
%canon_reldir%___glpk___la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(GLPK_CPPFLAGS)
%canon_reldir%___glpk___la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(GLPK_LDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%___glpk___la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(GLPK_LIBS) $(OCT_LINK_DEPS)

%canon_reldir%___init_fltk___la_SOURCES = %reldir%/__init_fltk__.cc
%canon_reldir%___init_fltk___la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(FLTK_CPPFLAGS) $(FT2_CPPFLAGS) $(FONTCONFIG_CPPFLAGS)
%canon_reldir%___init_fltk___la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(FLTK_CPPFLAGS) $(FT2_CPPFLAGS) $(FONTCONFIG_CPPFLAGS)
%canon_reldir%___init_fltk___la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(FLTK_CPPFLAGS) $(FT2_CPPFLAGS) $(FONTCONFIG_CPPFLAGS)
%canon_reldir%___init_fltk___la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(FLTK_LDFLAGS) $(FT2_LDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%___init_fltk___la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(FLTK_LIBS) $(FT2_LIBS) $(OPENGL_LIBS) $(OCT_LINK_DEPS)

%canon_reldir%___init_gnuplot___la_SOURCES = %reldir%/__init_gnuplot__.cc
%canon_reldir%___init_gnuplot___la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(FT2_CPPFLAGS) $(FONTCONFIG_CPPFLAGS)
%canon_reldir%___init_gnuplot___la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(FT2_CPPFLAGS) $(FONTCONFIG_CPPFLAGS)
%canon_reldir%___init_gnuplot___la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(FT2_CPPFLAGS) $(FONTCONFIG_CPPFLAGS)
%canon_reldir%___init_gnuplot___la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG)  $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%___init_gnuplot___la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la  $(OCT_LINK_DEPS)

%canon_reldir%___ode15___la_SOURCES = %reldir%/__ode15__.cc
%canon_reldir%___ode15___la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(SUNDIALS_XCPPFLAGS)
%canon_reldir%___ode15___la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(SUNDIALS_XCPPFLAGS)
%canon_reldir%___ode15___la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(SUNDIALS_XCPPFLAGS)
%canon_reldir%___ode15___la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(SUNDIALS_XLDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%___ode15___la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(SUNDIALS_XLIBS) $(OCT_LINK_DEPS)

%canon_reldir%___osmesa_print___la_SOURCES = %reldir%/__osmesa_print__.cc
%canon_reldir%___osmesa_print___la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(OSMESA_CPPFLAGS) $(FT2_CPPFLAGS)
%canon_reldir%___osmesa_print___la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(OSMESA_CPPFLAGS) $(FT2_CPPFLAGS)
%canon_reldir%___osmesa_print___la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(OSMESA_CPPFLAGS) $(FT2_CPPFLAGS)
%canon_reldir%___osmesa_print___la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(OSMESA_LDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%___osmesa_print___la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(OSMESA_LIBS) $(OCT_LINK_DEPS)

%canon_reldir%___voronoi___la_SOURCES = %reldir%/__voronoi__.cc
%canon_reldir%___voronoi___la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(QHULL_CPPFLAGS)
%canon_reldir%___voronoi___la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(QHULL_CPPFLAGS)
%canon_reldir%___voronoi___la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(QHULL_CPPFLAGS)
%canon_reldir%___voronoi___la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(QHULL_LDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%___voronoi___la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(QHULL_LIBS) $(OCT_LINK_DEPS)

%canon_reldir%_amd_la_SOURCES = %reldir%/amd.cc
%canon_reldir%_amd_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_amd_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_amd_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_amd_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(SPARSE_XLDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_amd_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(SPARSE_XLIBS) $(OCT_LINK_DEPS)

%canon_reldir%_audiodevinfo_la_SOURCES = %reldir%/audiodevinfo.cc
%canon_reldir%_audiodevinfo_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(PORTAUDIO_CPPFLAGS)
%canon_reldir%_audiodevinfo_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(PORTAUDIO_CPPFLAGS)
%canon_reldir%_audiodevinfo_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(PORTAUDIO_CPPFLAGS)
%canon_reldir%_audiodevinfo_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(PORTAUDIO_LDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_audiodevinfo_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(PORTAUDIO_LIBS) $(OCT_LINK_DEPS)

%canon_reldir%_audioread_la_SOURCES = %reldir%/audioread.cc
%canon_reldir%_audioread_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(SNDFILE_CPPFLAGS)
%canon_reldir%_audioread_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(SNDFILE_CPPFLAGS)
%canon_reldir%_audioread_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(SNDFILE_CPPFLAGS)
%canon_reldir%_audioread_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(SNDFILE_LDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_audioread_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(SNDFILE_LIBS) $(OCT_LINK_DEPS)

%canon_reldir%_ccolamd_la_SOURCES = %reldir%/ccolamd.cc
%canon_reldir%_ccolamd_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_ccolamd_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_ccolamd_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_ccolamd_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(SPARSE_XLDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_ccolamd_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(SPARSE_XLIBS) $(OCT_LINK_DEPS)

%canon_reldir%_chol_la_SOURCES = %reldir%/chol.cc
%canon_reldir%_chol_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_chol_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_chol_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_chol_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(SPARSE_XLDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_chol_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(SPARSE_XLIBS) $(OCT_LINK_DEPS)

%canon_reldir%_colamd_la_SOURCES = %reldir%/colamd.cc
%canon_reldir%_colamd_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_colamd_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_colamd_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_colamd_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(SPARSE_XLDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_colamd_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(SPARSE_XLIBS) $(OCT_LINK_DEPS)

%canon_reldir%_convhulln_la_SOURCES = %reldir%/convhulln.cc
%canon_reldir%_convhulln_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(QHULL_CPPFLAGS)
%canon_reldir%_convhulln_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(QHULL_CPPFLAGS)
%canon_reldir%_convhulln_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(QHULL_CPPFLAGS)
%canon_reldir%_convhulln_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(QHULL_LDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_convhulln_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(QHULL_LIBS) $(OCT_LINK_DEPS)

%canon_reldir%_dmperm_la_SOURCES = %reldir%/dmperm.cc
%canon_reldir%_dmperm_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_dmperm_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_dmperm_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_dmperm_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(SPARSE_XLDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_dmperm_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(SPARSE_XLIBS) $(OCT_LINK_DEPS)

%canon_reldir%_fftw_la_SOURCES = %reldir%/fftw.cc
%canon_reldir%_fftw_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(FFTW_XCPPFLAGS)
%canon_reldir%_fftw_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(FFTW_XCPPFLAGS)
%canon_reldir%_fftw_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(FFTW_XCPPFLAGS)
%canon_reldir%_fftw_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(FFTW_XLDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_fftw_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(FFTW_XLIBS) $(OCT_LINK_DEPS)

%canon_reldir%_gzip_la_SOURCES = %reldir%/gzip.cc
%canon_reldir%_gzip_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(Z_CPPFLAGS) $(BZ2_CPPFLAGS)
%canon_reldir%_gzip_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(Z_CPPFLAGS) $(BZ2_CPPFLAGS)
%canon_reldir%_gzip_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(Z_CPPFLAGS) $(BZ2_CPPFLAGS)
%canon_reldir%_gzip_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(Z_LDFLAGS) $(BZ2_LDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_gzip_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(Z_LIBS) $(BZ2_LIBS) $(OCT_LINK_DEPS)

%canon_reldir%_qr_la_SOURCES = %reldir%/qr.cc
%canon_reldir%_qr_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(QRUPDATE_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_qr_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(QRUPDATE_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_qr_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(QRUPDATE_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_qr_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(QRUPDATE_LDFLAGS) $(SPARSE_XLDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_qr_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(QRUPDATE_LIBS) $(SPARSE_XLIBS) $(OCT_LINK_DEPS)

%canon_reldir%_symbfact_la_SOURCES = %reldir%/symbfact.cc
%canon_reldir%_symbfact_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_symbfact_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_symbfact_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_symbfact_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(SPARSE_XLDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_symbfact_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(SPARSE_XLIBS) $(OCT_LINK_DEPS)

%canon_reldir%_symrcm_la_SOURCES = %reldir%/symrcm.cc
%canon_reldir%_symrcm_la_CPPFLAGS = $(libinterp_liboctinterp_la_CPPFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_symrcm_la_CFLAGS = $(libinterp_liboctinterp_la_CFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_symrcm_la_CXXFLAGS = $(libinterp_liboctinterp_la_CXXFLAGS) $(SPARSE_XCPPFLAGS)
%canon_reldir%_symrcm_la_LDFLAGS = -avoid-version -module $(NO_UNDEFINED_LDFLAG) $(SPARSE_XLDFLAGS) $(OCT_LINK_OPTS) $(WARN_LDFLAGS)
%canon_reldir%_symrcm_la_LIBADD = $(DLD_LIBOCTINTERP_LIBADD) liboctave/liboctave.la $(SPARSE_XLIBS) $(OCT_LINK_DEPS)

$(srcdir)/%reldir%/module.mk: $(srcdir)/%reldir%/config-module.sh $(srcdir)/%reldir%/config-module.awk $(srcdir)/%reldir%/module-files
	$(AM_V_GEN)$(SHELL) $(srcdir)/%reldir%/config-module.sh $(srcdir)

libinterp_MAINTAINERCLEANFILES += $(srcdir)/%reldir%/module.mk
