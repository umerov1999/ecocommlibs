package ink;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.geometry.Point;
import com.myscript.geometry.Rectangle;
import com.myscript.geometry.Transform;
import com.myscript.ink.Ink;
import com.myscript.ink.InkSelection;
import com.myscript.ink.InkStroke;
import com.myscript.ink.SelectionModifier;
import common.CFG;

/**
 * The <code>EditInk/code> example code shows you how to inspect and edit an
 * <code>Ink</code> object.
 */
public class EditInk
{
  /**
   * Path to the .jink file
   */
  private static final String JINK_FILE = CFG.DATA_DIR + "/flower.jink";

  /**
   * Main entry point for the <code>EditInk</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    final float floatTolerance = 0.001f;
    
    if (JINK_FILE == null || JINK_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for jink file. You can choose document filename by editing JINK_FILE static member variable of EditInk class");
      System.exit(-1);
    }

    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) loading an existing .jink file");
    final Ink ink = (Ink)Ink.load(engine, JINK_FILE);    
    System.out.println(" . " + new File(JINK_FILE).getCanonicalPath() + " loaded successfully");
    System.out.println();

    System.out.println("3) inspecting the content of the loaded Ink");
    System.out.println(" . the ink contains " + ink.getStrokeCount() + " strokes");
    System.out.println();

    System.out.println("4) inspecting the first stroke");
    final InkStroke stroke = ink.getStrokeAt(0);
    System.out.println(" . the first stroke contains " + stroke.getSampleCount() + " points");
    final Point[] pointArray = stroke.getPoints();
    System.out.println(" . coordinates of first point in the stroke: { " + pointArray[0].x + " ; " + pointArray[0].y + " }");
    System.out.println();

    System.out.println("5) selecting a portion of Ink that partially contains the first stroke");
    final InkSelection selection = InkSelection.create(ink);
    // select a portion of ink
    final Rectangle rectangle = new Rectangle(230, 300, 50, 50);
    selection.selectRectangle(rectangle, SelectionModifier.SET);
    // check that we selected something
    final int selectionStrokeCount = selection.getStrokeCount();
    System.out.println(" . selection successfully created, containing " + selectionStrokeCount + " strokes or substrokes");
    final InkStroke selectionStroke = selection.getStrokeAt(0);
    final Point[] point = new Point[1];
    selectionStroke.getPoints(0, 1, point);
    System.out.println(" . coordinates of first point in the selection: { " + point[0].x + " ; " + point[0].y + " }");
    System.out.println();

    System.out.println("6) transforming the selection");
    final Transform transform = new Transform();
    transform.setIdentity();
    final float tx = 10.0f;
    final float ty = 100.0f;
    transform.translate(tx, ty);
    ink.transform(selection, transform);
    final int strokeCountAfterTransform = ink.getStrokeCount();
    System.out.println(" . selection successfully transformed; now the Ink has " + strokeCountAfterTransform + " strokes");
    final InkStroke transformedStroke = selection.getStrokeAt(0);
    final Point[] transformedPoint = new Point[1];
    transformedStroke.getPoints(0, 1, transformedPoint);
    if (!((1.0f - floatTolerance) * transformedPoint[0].x <= tx + point[0].x && (1.0f + floatTolerance) * transformedPoint[0].x >= tx + point[0].x))
    {
      System.err.println(" . wrong coordinates of first point in the selection after transform: { " + transformedPoint[0].x + " ; " + transformedPoint[0].y + " }");
      System.exit(-1);
    }
    System.out.println(" . coordinates of first point in the selection after transform: { " + transformedPoint[0].x + " ; " + transformedPoint[0].y + " }");
    System.out.println();

    System.out.println("7) erasing selection");
    ink.erase(selection);
    final int strokeCountAfterDeletion = ink.getStrokeCount();
    if (strokeCountAfterTransform - selectionStrokeCount != strokeCountAfterDeletion)
    {
      System.err.println(" . selection unsuccessfully erased; now the Ink has " + strokeCountAfterDeletion + " strokes");
      System.exit(-1);
    }
    System.out.println(" . selection successfully erased; now the Ink has " + strokeCountAfterDeletion + " strokes");
    System.out.println();

    System.out.println("8) inspecting the new first stroke");
    final InkStroke newStroke = ink.getStrokeAt(0);
    final Point[] newPointArray = new Point[newStroke.getSampleCount()];
    newStroke.getPoints(0, 1, newPointArray);
    System.out.println(" . coordinates of first point in the stroke: { " + newPointArray[0].x + " ; " + newPointArray[0].y + " }");
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("9) cleaning-up: disposing of objects and disposing of the Engine");
    newStroke.dispose();
    transformedStroke.dispose();
    selectionStroke.dispose();
    selection.dispose();
    stroke.dispose();
    ink.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to inspect and edit ink");
  }

} // EditInk
