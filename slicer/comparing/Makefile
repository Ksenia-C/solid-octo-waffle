MAKE_ASSIGNMENT_OPT ?= 0
PRINTING ?= 1
KEY_DISTRIBUTION ?= 0

run:
	$(info Running with MAKE_ASSIGNMENT_OPT=$(MAKE_ASSIGNMENT_OPT) and PRINTING=$(PRINTING) and KEY_DISTRIBUTION=$(KEY_DISTRIBUTION))
	MAKE_ASSIGNMENT_OPT=$(MAKE_ASSIGNMENT_OPT) PRINTING=$(PRINTING) KEY_DISTRIBUTION=$(KEY_DISTRIBUTION) ../build/SlicerSimple


run_eval:
	$(info Running all with KEY_DISTRIBUTION=$(KEY_DISTRIBUTION))
	MAKE_ASSIGNMENT_OPT=0 PRINTING=2 KEY_DISTRIBUTION=$(KEY_DISTRIBUTION) ../build/SlicerSimple 1>article_output
	MAKE_ASSIGNMENT_OPT=1 PRINTING=2 KEY_DISTRIBUTION=$(KEY_DISTRIBUTION) ../build/SlicerSimple 1>petrenko_output


# MAKE_ASSIGNMENT_OPT=0 PRINTING=0 KEY_DISTRIBUTION=0 ../build/SlicerSimple
# MAKE_ASSIGNMENT_OPT:
# 	0 = article
# 	1 = petrenko
# PRINTING:
# 	0 = active tasks at workers
# 	1 = key distribution
# 	2 = disbalance
# KEY_DISTRIBUTION:
# 	0 = simple even
# 	1 = simple static hot at the middle
#   2 = simple floating mean with normal distr
