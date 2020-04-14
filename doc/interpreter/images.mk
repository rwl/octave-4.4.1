## DO NOT EDIT -- generated from module-files by images.awk

if AMCOND_BUILD_DOCS
DOC_IMAGES_SRC += doc/interpreter/geometryimages.m
GEOMETRYIMAGES_EPS = doc/interpreter/voronoi.eps doc/interpreter/triplot.eps doc/interpreter/griddata.eps doc/interpreter/convhull.eps doc/interpreter/delaunay.eps doc/interpreter/inpolygon.eps
BUILT_DOC_IMAGES_EPS += $(GEOMETRYIMAGES_EPS)
doc/interpreter/voronoi.eps: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'voronoi', 'eps');"
doc/interpreter/triplot.eps: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'triplot', 'eps');"
doc/interpreter/griddata.eps: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'griddata', 'eps');"
doc/interpreter/convhull.eps: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'convhull', 'eps');"
doc/interpreter/delaunay.eps: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'delaunay', 'eps');"
doc/interpreter/inpolygon.eps: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'inpolygon', 'eps');"
GEOMETRYIMAGES_PDF = doc/interpreter/voronoi.pdf doc/interpreter/triplot.pdf doc/interpreter/griddata.pdf doc/interpreter/convhull.pdf doc/interpreter/delaunay.pdf doc/interpreter/inpolygon.pdf
BUILT_DOC_IMAGES_PDF += $(GEOMETRYIMAGES_PDF)
doc/interpreter/voronoi.pdf: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'voronoi', 'pdf');"
doc/interpreter/triplot.pdf: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'triplot', 'pdf');"
doc/interpreter/griddata.pdf: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'griddata', 'pdf');"
doc/interpreter/convhull.pdf: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'convhull', 'pdf');"
doc/interpreter/delaunay.pdf: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'delaunay', 'pdf');"
doc/interpreter/inpolygon.pdf: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'inpolygon', 'pdf');"
GEOMETRYIMAGES_PNG = doc/interpreter/voronoi.png doc/interpreter/triplot.png doc/interpreter/griddata.png doc/interpreter/convhull.png doc/interpreter/delaunay.png doc/interpreter/inpolygon.png
BUILT_DOC_IMAGES_PNG += $(GEOMETRYIMAGES_PNG)
doc/interpreter/voronoi.png: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'voronoi', 'png');"
doc/interpreter/triplot.png: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'triplot', 'png');"
doc/interpreter/griddata.png: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'griddata', 'png');"
doc/interpreter/convhull.png: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'convhull', 'png');"
doc/interpreter/delaunay.png: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'delaunay', 'png');"
doc/interpreter/inpolygon.png: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'inpolygon', 'png');"
GEOMETRYIMAGES_TXT = doc/interpreter/voronoi.txt doc/interpreter/triplot.txt doc/interpreter/griddata.txt doc/interpreter/convhull.txt doc/interpreter/delaunay.txt doc/interpreter/inpolygon.txt
BUILT_DOC_IMAGES_TXT += $(GEOMETRYIMAGES_TXT)
doc/interpreter/voronoi.txt: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'voronoi', 'txt');"
doc/interpreter/triplot.txt: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'triplot', 'txt');"
doc/interpreter/griddata.txt: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'griddata', 'txt');"
doc/interpreter/convhull.txt: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'convhull', 'txt');"
doc/interpreter/delaunay.txt: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'delaunay', 'txt');"
doc/interpreter/inpolygon.txt: doc/interpreter/geometryimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "geometryimages ('doc/interpreter/', 'inpolygon', 'txt');"
DOC_IMAGES_SRC += doc/interpreter/interpimages.m
INTERPIMAGES_EPS = doc/interpreter/interpft.eps doc/interpreter/interpn.eps doc/interpreter/interpderiv1.eps doc/interpreter/interpderiv2.eps
BUILT_DOC_IMAGES_EPS += $(INTERPIMAGES_EPS)
doc/interpreter/interpft.eps: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpft', 'eps');"
doc/interpreter/interpn.eps: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpn', 'eps');"
doc/interpreter/interpderiv1.eps: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpderiv1', 'eps');"
doc/interpreter/interpderiv2.eps: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpderiv2', 'eps');"
INTERPIMAGES_PDF = doc/interpreter/interpft.pdf doc/interpreter/interpn.pdf doc/interpreter/interpderiv1.pdf doc/interpreter/interpderiv2.pdf
BUILT_DOC_IMAGES_PDF += $(INTERPIMAGES_PDF)
doc/interpreter/interpft.pdf: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpft', 'pdf');"
doc/interpreter/interpn.pdf: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpn', 'pdf');"
doc/interpreter/interpderiv1.pdf: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpderiv1', 'pdf');"
doc/interpreter/interpderiv2.pdf: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpderiv2', 'pdf');"
INTERPIMAGES_PNG = doc/interpreter/interpft.png doc/interpreter/interpn.png doc/interpreter/interpderiv1.png doc/interpreter/interpderiv2.png
BUILT_DOC_IMAGES_PNG += $(INTERPIMAGES_PNG)
doc/interpreter/interpft.png: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpft', 'png');"
doc/interpreter/interpn.png: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpn', 'png');"
doc/interpreter/interpderiv1.png: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpderiv1', 'png');"
doc/interpreter/interpderiv2.png: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpderiv2', 'png');"
INTERPIMAGES_TXT = doc/interpreter/interpft.txt doc/interpreter/interpn.txt doc/interpreter/interpderiv1.txt doc/interpreter/interpderiv2.txt
BUILT_DOC_IMAGES_TXT += $(INTERPIMAGES_TXT)
doc/interpreter/interpft.txt: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpft', 'txt');"
doc/interpreter/interpn.txt: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpn', 'txt');"
doc/interpreter/interpderiv1.txt: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpderiv1', 'txt');"
doc/interpreter/interpderiv2.txt: doc/interpreter/interpimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "interpimages ('doc/interpreter/', 'interpderiv2', 'txt');"
DOC_IMAGES_SRC += doc/interpreter/plotimages.m
PLOTIMAGES_EPS = doc/interpreter/plot.eps doc/interpreter/hist.eps doc/interpreter/errorbar.eps doc/interpreter/polar.eps doc/interpreter/mesh.eps doc/interpreter/plot3.eps doc/interpreter/extended.eps
BUILT_DOC_IMAGES_EPS += $(PLOTIMAGES_EPS)
doc/interpreter/plot.eps: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'plot', 'eps');"
doc/interpreter/hist.eps: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'hist', 'eps');"
doc/interpreter/errorbar.eps: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'errorbar', 'eps');"
doc/interpreter/polar.eps: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'polar', 'eps');"
doc/interpreter/mesh.eps: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'mesh', 'eps');"
doc/interpreter/plot3.eps: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'plot3', 'eps');"
doc/interpreter/extended.eps: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'extended', 'eps');"
PLOTIMAGES_PDF = doc/interpreter/plot.pdf doc/interpreter/hist.pdf doc/interpreter/errorbar.pdf doc/interpreter/polar.pdf doc/interpreter/mesh.pdf doc/interpreter/plot3.pdf doc/interpreter/extended.pdf
BUILT_DOC_IMAGES_PDF += $(PLOTIMAGES_PDF)
doc/interpreter/plot.pdf: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'plot', 'pdf');"
doc/interpreter/hist.pdf: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'hist', 'pdf');"
doc/interpreter/errorbar.pdf: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'errorbar', 'pdf');"
doc/interpreter/polar.pdf: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'polar', 'pdf');"
doc/interpreter/mesh.pdf: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'mesh', 'pdf');"
doc/interpreter/plot3.pdf: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'plot3', 'pdf');"
doc/interpreter/extended.pdf: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'extended', 'pdf');"
PLOTIMAGES_PNG = doc/interpreter/plot.png doc/interpreter/hist.png doc/interpreter/errorbar.png doc/interpreter/polar.png doc/interpreter/mesh.png doc/interpreter/plot3.png doc/interpreter/extended.png
BUILT_DOC_IMAGES_PNG += $(PLOTIMAGES_PNG)
doc/interpreter/plot.png: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'plot', 'png');"
doc/interpreter/hist.png: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'hist', 'png');"
doc/interpreter/errorbar.png: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'errorbar', 'png');"
doc/interpreter/polar.png: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'polar', 'png');"
doc/interpreter/mesh.png: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'mesh', 'png');"
doc/interpreter/plot3.png: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'plot3', 'png');"
doc/interpreter/extended.png: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'extended', 'png');"
PLOTIMAGES_TXT = doc/interpreter/plot.txt doc/interpreter/hist.txt doc/interpreter/errorbar.txt doc/interpreter/polar.txt doc/interpreter/mesh.txt doc/interpreter/plot3.txt doc/interpreter/extended.txt
BUILT_DOC_IMAGES_TXT += $(PLOTIMAGES_TXT)
doc/interpreter/plot.txt: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'plot', 'txt');"
doc/interpreter/hist.txt: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'hist', 'txt');"
doc/interpreter/errorbar.txt: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'errorbar', 'txt');"
doc/interpreter/polar.txt: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'polar', 'txt');"
doc/interpreter/mesh.txt: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'mesh', 'txt');"
doc/interpreter/plot3.txt: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'plot3', 'txt');"
doc/interpreter/extended.txt: doc/interpreter/plotimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "plotimages ('doc/interpreter/', 'extended', 'txt');"
DOC_IMAGES_SRC += doc/interpreter/sparseimages.m
SPARSEIMAGES_EPS = doc/interpreter/gplot.eps doc/interpreter/grid.eps doc/interpreter/spmatrix.eps doc/interpreter/spchol.eps doc/interpreter/spcholperm.eps
BUILT_DOC_IMAGES_EPS += $(SPARSEIMAGES_EPS)
doc/interpreter/gplot.eps: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'gplot', 'eps');"
doc/interpreter/grid.eps: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'grid', 'eps');"
doc/interpreter/spmatrix.eps: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spmatrix', 'eps');"
doc/interpreter/spchol.eps: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spchol', 'eps');"
doc/interpreter/spcholperm.eps: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spcholperm', 'eps');"
SPARSEIMAGES_PDF = doc/interpreter/gplot.pdf doc/interpreter/grid.pdf doc/interpreter/spmatrix.pdf doc/interpreter/spchol.pdf doc/interpreter/spcholperm.pdf
BUILT_DOC_IMAGES_PDF += $(SPARSEIMAGES_PDF)
doc/interpreter/gplot.pdf: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'gplot', 'pdf');"
doc/interpreter/grid.pdf: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'grid', 'pdf');"
doc/interpreter/spmatrix.pdf: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spmatrix', 'pdf');"
doc/interpreter/spchol.pdf: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spchol', 'pdf');"
doc/interpreter/spcholperm.pdf: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spcholperm', 'pdf');"
SPARSEIMAGES_PNG = doc/interpreter/gplot.png doc/interpreter/grid.png doc/interpreter/spmatrix.png doc/interpreter/spchol.png doc/interpreter/spcholperm.png
BUILT_DOC_IMAGES_PNG += $(SPARSEIMAGES_PNG)
doc/interpreter/gplot.png: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'gplot', 'png');"
doc/interpreter/grid.png: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'grid', 'png');"
doc/interpreter/spmatrix.png: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spmatrix', 'png');"
doc/interpreter/spchol.png: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spchol', 'png');"
doc/interpreter/spcholperm.png: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spcholperm', 'png');"
SPARSEIMAGES_TXT = doc/interpreter/gplot.txt doc/interpreter/grid.txt doc/interpreter/spmatrix.txt doc/interpreter/spchol.txt doc/interpreter/spcholperm.txt
BUILT_DOC_IMAGES_TXT += $(SPARSEIMAGES_TXT)
doc/interpreter/gplot.txt: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'gplot', 'txt');"
doc/interpreter/grid.txt: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'grid', 'txt');"
doc/interpreter/spmatrix.txt: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spmatrix', 'txt');"
doc/interpreter/spchol.txt: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spchol', 'txt');"
doc/interpreter/spcholperm.txt: doc/interpreter/sparseimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "sparseimages ('doc/interpreter/', 'spcholperm', 'txt');"
DOC_IMAGES_SRC += doc/interpreter/splineimages.m
SPLINEIMAGES_EPS = doc/interpreter/splinefit1.eps doc/interpreter/splinefit2.eps doc/interpreter/splinefit3.eps doc/interpreter/splinefit4.eps doc/interpreter/splinefit6.eps
BUILT_DOC_IMAGES_EPS += $(SPLINEIMAGES_EPS)
doc/interpreter/splinefit1.eps: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit1', 'eps');"
doc/interpreter/splinefit2.eps: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit2', 'eps');"
doc/interpreter/splinefit3.eps: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit3', 'eps');"
doc/interpreter/splinefit4.eps: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit4', 'eps');"
doc/interpreter/splinefit6.eps: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit6', 'eps');"
SPLINEIMAGES_PDF = doc/interpreter/splinefit1.pdf doc/interpreter/splinefit2.pdf doc/interpreter/splinefit3.pdf doc/interpreter/splinefit4.pdf doc/interpreter/splinefit6.pdf
BUILT_DOC_IMAGES_PDF += $(SPLINEIMAGES_PDF)
doc/interpreter/splinefit1.pdf: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit1', 'pdf');"
doc/interpreter/splinefit2.pdf: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit2', 'pdf');"
doc/interpreter/splinefit3.pdf: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit3', 'pdf');"
doc/interpreter/splinefit4.pdf: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit4', 'pdf');"
doc/interpreter/splinefit6.pdf: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit6', 'pdf');"
SPLINEIMAGES_PNG = doc/interpreter/splinefit1.png doc/interpreter/splinefit2.png doc/interpreter/splinefit3.png doc/interpreter/splinefit4.png doc/interpreter/splinefit6.png
BUILT_DOC_IMAGES_PNG += $(SPLINEIMAGES_PNG)
doc/interpreter/splinefit1.png: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit1', 'png');"
doc/interpreter/splinefit2.png: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit2', 'png');"
doc/interpreter/splinefit3.png: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit3', 'png');"
doc/interpreter/splinefit4.png: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit4', 'png');"
doc/interpreter/splinefit6.png: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit6', 'png');"
SPLINEIMAGES_TXT = doc/interpreter/splinefit1.txt doc/interpreter/splinefit2.txt doc/interpreter/splinefit3.txt doc/interpreter/splinefit4.txt doc/interpreter/splinefit6.txt
BUILT_DOC_IMAGES_TXT += $(SPLINEIMAGES_TXT)
doc/interpreter/splinefit1.txt: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit1', 'txt');"
doc/interpreter/splinefit2.txt: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit2', 'txt');"
doc/interpreter/splinefit3.txt: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit3', 'txt');"
doc/interpreter/splinefit4.txt: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit4', 'txt');"
doc/interpreter/splinefit6.txt: doc/interpreter/splineimages.m
	$(AM_V_GEN)$(SHELL) run-octave --norc --silent --no-history --path $(abs_top_srcdir)/doc/interpreter/ --eval "splineimages ('doc/interpreter/', 'splinefit6', 'txt');"
endif

doc_MAINTAINERCLEANFILES += $(srcdir)/%reldir%/images.mk
