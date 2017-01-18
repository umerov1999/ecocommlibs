package common;

import java.io.PrintStream;

import com.myscript.engine.IProgress;

/**
 * The <code>TextUIProgressBar</code> class implements a progress bar for use in
 * a text user interface based application.
 *
 * <p>Typical usage:
<pre>
  TextUIProgressBar progressBar = new TextUIProgressBar(System.out);
  progressBar.update(0, 100);   // prints 0% status
  task.run(progressBar);
  progressBar.update(100, 100); // prints 100% status
  System.out.println();         // finalizes printing
</pre>
 * where <code>task</code> implements
 * <code>com.myscript.engine.IRunnable</code>.
 */
public class TextUIProgressBar
    implements IProgress
{
  /**
   * Creates a new <code>TextUIProgressBar</code> that will print to the
   * specified output <code>PrintStream</code>.
   * 
   * @param out The output <code>PrintStream</code>.
   */
  public TextUIProgressBar(final PrintStream out)
  {
    this.out = out;
    this.redraw = true;
    this.cursorColumn = 0;
  }

  /**
   * Performs update to the output <code>PrintStream</code> to reflect progress
   * status.
   */
  public final boolean update(int amountDone, int amountToDo)
  {
    boolean doFlush = false;
    int newCursorColumn;
    if (redraw)
    {
      out.println("  0% [                                                  ] 100%");
      out.print("      ");
      cursorColumn = 0;
      redraw = false;
      doFlush = true;
    }
    newCursorColumn = (int)(50.0f * (float)amountDone / (float)amountToDo + 0.5f);
    if (newCursorColumn > 50)
      newCursorColumn = 50;
    for (; cursorColumn < newCursorColumn; ++cursorColumn)
    {
      out.print('^');
      doFlush = true;
    }
    if (doFlush)
      out.flush();
    return true; // continue run
  }
  
  private final PrintStream out;
  private boolean redraw;
  private int cursorColumn;
  
} // TextUIProgressBar
