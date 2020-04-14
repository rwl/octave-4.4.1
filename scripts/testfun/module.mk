FCN_FILE_DIRS += scripts/testfun

%canon_reldir%_PRIVATE_FCN_FILES = \
  %reldir%/private/compare_plot_demos.m \
  %reldir%/private/dump_demos.m \
  %reldir%/private/html_compare_plot_demos.m \
  %reldir%/private/html_plot_demos_template.html

%canon_reldir%_FCN_FILES = \
  %reldir%/__have_feature__.m \
  %reldir%/__printf_assert__.m \
  %reldir%/__prog_output_assert__.m \
  %reldir%/__run_test_suite__.m \
  %reldir%/assert.m \
  %reldir%/demo.m \
  %reldir%/example.m \
  %reldir%/fail.m \
  %reldir%/rundemos.m \
  %reldir%/runtests.m \
  %reldir%/speed.m \
  %reldir%/test.m

%canon_reldir%dir = $(fcnfiledir)/testfun

%canon_reldir%_DATA = $(%canon_reldir%_FCN_FILES)

%canon_reldir%_privatedir = $(fcnfiledir)/testfun/private

%canon_reldir%_private_DATA = $(%canon_reldir%_PRIVATE_FCN_FILES)

FCN_FILES += \
  $(%canon_reldir%_FCN_FILES) \
  $(%canon_reldir%_PRIVATE_FCN_FILES)

PKG_ADD_FILES += %reldir%/PKG_ADD

DIRSTAMP_FILES += %reldir%/$(octave_dirstamp)
