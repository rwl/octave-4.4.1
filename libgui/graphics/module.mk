if AMCOND_BUILD_QT_GRAPHICS

OCTAVE_GUI_GRAPHICS_MOC = \
  %reldir%/moc-annotation-dialog.cc \
  %reldir%/moc-Backend.cc \
  %reldir%/moc-ButtonControl.cc \
  %reldir%/moc-ButtonGroup.cc \
  %reldir%/moc-ContextMenu.cc \
  %reldir%/moc-EditControl.cc \
  %reldir%/moc-Figure.cc \
  %reldir%/moc-FigureWindow.cc \
  %reldir%/moc-ListBoxControl.cc \
  %reldir%/moc-Menu.cc \
  %reldir%/moc-MouseModeActionGroup.cc \
  %reldir%/moc-Object.cc \
  %reldir%/moc-ObjectFactory.cc \
  %reldir%/moc-ObjectProxy.cc \
  %reldir%/moc-PopupMenuControl.cc \
  %reldir%/moc-PushTool.cc \
  %reldir%/moc-SliderControl.cc \
  %reldir%/moc-TextEdit.cc \
  %reldir%/moc-ToggleTool.cc \
  %reldir%/moc-ToolBar.cc

$(OCTAVE_GUI_GRAPHICS_MOC): | %reldir%/$(octave_dirstamp)

DIRSTAMP_FILES += \
  %reldir%/$(octave_dirstamp)

octave_gui_MOC += \
  $(OCTAVE_GUI_GRAPHICS_MOC)

octave_gui_graphics_UI = \
  %reldir%/annotation-dialog.ui

octave_gui_graphics_UI_H = $(patsubst %reldir%/%.ui, %reldir%/ui-%.h, $(octave_gui_graphics_UI))

$(octave_gui_graphics_UI_H): | %reldir%/$(octave_dirstamp)

BUILT_SOURCES += $(octave_gui_graphics_UI_H)

octave_gui_graphics_RC = %reldir%/qrc-qthandles.cc

$(octave_gui_graphics_RC): | %reldir%/$(octave_dirstamp)

noinst_HEADERS += \
  %reldir%/__init_qt__.h \
  %reldir%/annotation-dialog.h \
  %reldir%/Backend.h \
  %reldir%/BaseControl.h \
  %reldir%/ButtonControl.h \
  %reldir%/ButtonGroup.h \
  %reldir%/Canvas.h \
  %reldir%/CheckBoxControl.h \
  %reldir%/Container.h \
  %reldir%/ContextMenu.h \
  %reldir%/EditControl.h \
  %reldir%/Figure.h \
  %reldir%/FigureWindow.h \
  %reldir%/GenericEventNotify.h \
  %reldir%/GLCanvas.h \
  %reldir%/KeyMap.h \
  %reldir%/ListBoxControl.h \
  %reldir%/Logger.h \
  %reldir%/Menu.h \
  %reldir%/MenuContainer.h \
  %reldir%/MouseModeActionGroup.h \
  %reldir%/Object.h \
  %reldir%/ObjectFactory.h \
  %reldir%/ObjectProxy.h \
  %reldir%/Panel.h \
  %reldir%/PopupMenuControl.h \
  %reldir%/PushButtonControl.h \
  %reldir%/PushTool.h \
  %reldir%/QtHandlesUtils.h \
  %reldir%/RadioButtonControl.h \
  %reldir%/SliderControl.h \
  %reldir%/TextControl.h \
  %reldir%/TextEdit.h \
  %reldir%/ToggleButtonControl.h \
  %reldir%/ToggleTool.h \
  %reldir%/ToolBar.h \
  %reldir%/ToolBarButton.h \
  %reldir%/gl-select.h \
  $(TEMPLATE_SRC)

%canon_reldir%_%canon_reldir%_la_SOURCES = \
  %reldir%/__init_qt__.cc \
  %reldir%/annotation-dialog.cc \
  %reldir%/Backend.cc \
  %reldir%/BaseControl.cc \
  %reldir%/ButtonControl.cc \
  %reldir%/ButtonGroup.cc \
  %reldir%/Canvas.cc \
  %reldir%/CheckBoxControl.cc \
  %reldir%/Container.cc \
  %reldir%/ContextMenu.cc \
  %reldir%/EditControl.cc \
  %reldir%/Figure.cc \
  %reldir%/FigureWindow.cc \
  %reldir%/GLCanvas.cc \
  %reldir%/KeyMap.cc \
  %reldir%/ListBoxControl.cc \
  %reldir%/Logger.cc \
  %reldir%/Menu.cc \
  %reldir%/MouseModeActionGroup.cc \
  %reldir%/Object.cc \
  %reldir%/ObjectFactory.cc \
  %reldir%/ObjectProxy.cc \
  %reldir%/Panel.cc \
  %reldir%/PopupMenuControl.cc \
  %reldir%/PushButtonControl.cc \
  %reldir%/PushTool.cc \
  %reldir%/QtHandlesUtils.cc \
  %reldir%/RadioButtonControl.cc \
  %reldir%/SliderControl.cc \
  %reldir%/TextControl.cc \
  %reldir%/TextEdit.cc \
  %reldir%/ToggleButtonControl.cc \
  %reldir%/ToggleTool.cc \
  %reldir%/ToolBar.cc \
  %reldir%/gl-select.cc

TEMPLATE_SRC = \
  %reldir%/ToolBarButton.cc

nodist_%canon_reldir%_%canon_reldir%_la_SOURCES = $(octave_gui_graphics_MOC) $(octave_gui_graphics_RC)

%canon_reldir%_%canon_reldir%_la_CPPFLAGS = \
  $(AM_CPPFLAGS) \
  $(FT2_CPPFLAGS) \
  $(FONTCONFIG_CPPFLAGS) \
  $(HDF5_CPPFLAGS) \
  @OCTGUI_DLL_DEFS@ \
  @QT_CPPFLAGS@ \
  -Ilibgui/graphics -I$(srcdir)/libgui/graphics \
  -Isrc -I$(srcdir)/libgui/src \
  -I$(srcdir)/liboctave/array \
  -Iliboctave/numeric -I$(srcdir)/liboctave/numeric \
  -Iliboctave/operators -I$(srcdir)/liboctave/operators \
  -I$(srcdir)/liboctave/system \
  -I$(srcdir)/liboctave/util \
  -Ilibinterp -I$(srcdir)/libinterp \
  -Ilibinterp/parse-tree -I$(srcdir)/libinterp/parse-tree \
  -Ilibinterp/corefcn -I$(srcdir)/libinterp/corefcn \
  -I$(srcdir)/libinterp/octave-value

%canon_reldir%_%canon_reldir%_la_CFLAGS = $(AM_CFLAGS) $(WARN_CFLAGS)

%canon_reldir%_%canon_reldir%_la_CXXFLAGS = $(AM_CXXFLAGS) $(WARN_CXXFLAGS)

noinst_LTLIBRARIES += %reldir%/libgui-graphics.la

libgui_EXTRA_DIST += \
  %reldir%/qthandles.qrc \
  %reldir%/images/README \
  %reldir%/images/pan.png \
  %reldir%/images/rotate.png \
  %reldir%/images/select.png \
  %reldir%/images/zoom-in.png \
  %reldir%/images/zoom-out.png \
  $(octave_gui_graphics_UI)

libgui_CLEANFILES += \
  $(octave_gui_graphics_MOC) \
  $(octave_gui_graphics_RC) \
  $(octave_gui_graphics_UI_H)

endif
