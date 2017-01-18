package ink;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.ink.Ink;
import com.myscript.ink.InkStroke;
import com.myscript.ink.InkStrokeBuilder;
import com.myscript.ink.InkStrokeFormat;
import com.myscript.ink.InkStrokeFormatBuilder;
import common.CFG;

/**
 * The <code>CreateInk</code> example code shows you how to create, fill and
 * serialize ink with an <code>Ink</code> object.
 */
public class CreateInk
{
  /**
   * Path to the .jink file
   */
  private static final String JINK_FILE = CFG.OUTPUT_DIR + "/CreateInk-output.jink";

  /**
   * Format of the stroke
   */
  private static final String STROKE_FORMAT = "{\"channels\":[{\"name\":\"X\"},{\"name\":\"Y\"}]}";

  /**
   * Main entry point for the <code>CreateInk</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (JINK_FILE == null || JINK_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for jink file. You can choose document filename by editing JINK_FILE static member variable of CreateInk class");
      System.exit(-1);
    }

    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating an empty Ink object");
    final Ink ink = Ink.create(engine);
    System.out.println(" . Ink created successfully");
    System.out.println();

    System.out.println("3) creating a new stroke builder");
    final InkStrokeFormat strokeFormat = InkStrokeFormatBuilder.parseInkStrokeFormat(engine, STROKE_FORMAT);
    final InkStrokeBuilder strokeBuilder = InkStrokeBuilder.create(strokeFormat);
    System.out.println(" . stroke builder created successfully");
    System.out.println();

    System.out.println("4) filling the stroke builder with samples");
    strokeBuilder.clear();
    final float[][] values =  { { 41.0f, 60.0f },
                                { 40.0f, 100.0f },
                                { 38.0f,  64.0f },
                                { 47.0f,  60.0f },
                                { 55.0f,  70.0f },
                                { 52.0f,  80.0f },
                                { 42.0f,  79.0f } };
    strokeBuilder.addSample(values[0]);
    strokeBuilder.addSample(values[1]);
    strokeBuilder.addSample(values[2]);
    strokeBuilder.addSample(values[3]);
    strokeBuilder.addSample(values[4]);
    strokeBuilder.addSample(values[5]);
    strokeBuilder.addSample(values[6]);
    System.out.println(" . stroke builder succesfully filled with " + strokeBuilder.getSampleCount() + " points.");
    System.out.println();
    
    System.out.println("5) creating a new stroke and adding it to the Ink");
    final InkStroke stroke = strokeBuilder.createStroke();
    System.out.println(" . stroke created successfully");
    System.out.println(" . the Ink still contains " + ink.getStrokeCount() + " stroke");
    ink.addStroke(stroke);
    System.out.println(" . stroke successfully added");
    System.out.println(" . the Ink now contains " + ink.getStrokeCount() + " stroke");
    System.out.println();
    
    System.out.println("6) storing the Ink to disk");
    ink.store(JINK_FILE);
    System.out.println(" . Ink stored successfully to: " + new File(JINK_FILE).getCanonicalPath());
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("7) cleaning-up: disposing of objects and disposing of the Engine");
    stroke.dispose();
    strokeBuilder.dispose();
    strokeFormat.dispose();
    ink.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to create, fill and serialize ink");
  }

} // CreateInk
