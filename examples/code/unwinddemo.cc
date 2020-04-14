#include <octave/oct.h>
#include <octave/unwind-prot.h>

void
my_err_handler (const char *fmt, ...)
{
  // Do nothing!!
}

void
my_err_with_id_handler (const char *id, const char *fmt, ...)
{
  // Do nothing!!
}

DEFUN_DLD (unwinddemo, args, nargout, "Unwind Demo")
{
  if (args.length () < 2)
    print_usage ();

  NDArray a = args(0).array_value ();
  NDArray b = args(1).array_value ();

  // Declare unwind_protect frame which lasts as long as
  // the variable frame has scope.
  octave::unwind_protect frame;
  frame.add_fcn (set_liboctave_warning_handler,
                 current_liboctave_warning_handler);

  frame.add_fcn (set_liboctave_warning_with_id_handler,
                 current_liboctave_warning_with_id_handler);

  set_liboctave_warning_handler (my_err_handler);
  set_liboctave_warning_with_id_handler (my_err_with_id_handler);

  return octave_value (quotient (a, b));
}
