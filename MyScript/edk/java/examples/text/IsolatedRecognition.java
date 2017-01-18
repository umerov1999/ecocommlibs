package text;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.engine.FloatStructuredInput;
import com.myscript.engine.InputUnit;
import com.myscript.geometry.Rectangle;
import com.myscript.ink.Ink;
import com.myscript.ink.InkSelection;
import com.myscript.ink.InkStroke;
import com.myscript.ink.SelectionModifier;
import com.myscript.text.CandidateIterator;
import com.myscript.text.RecognitionResult;
import com.myscript.text.Resource;
import com.myscript.text.StructuredInputRecognizer;
import common.CFG;

/**
 * The <code>IsolatedRecognition</code> example code shows you how to
 * perform handwriting recognition on isolated characters digital ink.
 */
public class IsolatedRecognition
{
  /**
   * Represents a form field.
   */
  public static final class FormField
  {
    public float x0;         // x position of the first cell
    public float y0;         // y position of the first cell
    public float cellWidth;  // cell width
    public float cellHeight; // cell height
    public float padding;    // padding
    public int rowCount;     // number of rows in the form
    public int columnCount;  // number of columns in the form
  }
  
  /**
   * Main entry point for the <code>IsolatedRecognition</code> example
   * program.
   */
  public static final void main(String[] args) throws IOException
  {
    if (args.length == 0)
    {
      args = new String[1];
      args[0] = CFG.DATA_DIR + "/hello_world_isolated.ink";
    }
    
    // -- initialize the form field
    FormField formField = new FormField();
    formField.x0 = 210.0f;
    formField.y0 = 425.0f;
    formField.cellWidth = 50.0f;
    formField.cellHeight = 60.0f;
    formField.padding = 5.0f;
    formField.columnCount = 12;
    formField.rowCount = 1;
    
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a new StructuredInputRecognizer");
    final StructuredInputRecognizer recognizer = StructuredInputRecognizer.create(engine);
    System.out.println(" . StructuredInputRecognizer created successfully");
    System.out.println();

    System.out.println("3) loading and attaching resources to the recognizer");
    final Resource akIsolated = (Resource)EngineObject.load(engine, CFG.getAkIsoRes());
    System.out.println(" . CursiveAlphabetKnowledge resource loaded successfully");
    recognizer.attach(akIsolated);
    System.out.println(" . CursiveAlphabetKnowledge resource attached successfully");
    System.out.println(" . disposing of the CursiveAlphabetKnowledge resource (not needed anymore)");
    akIsolated.dispose();
    System.out.println(" . CursiveAlphabetKnowledge resource disposed of successfully");
    final Resource lkText = (Resource)EngineObject.load(engine, CFG.getLkTextRes());
    System.out.println(" . Main LinguisticKnowledge resource loaded successfully");
    recognizer.attach(lkText);
    System.out.println(" . Main LinguisticKnowledge resource attached successfully");
    System.out.println(" . disposing of the main LinguisticKnowledge resource (not needed anymore)");
    lkText.dispose();
    System.out.println(" . Main LinguisticKnowledge resource disposed of successfully");
    if (!CFG.getLang().contains("en"))
    {
      final String path;

      if (CFG.getLang().contains("zh") || CFG.getLang().contains("ja") || CFG.getLang().contains("ko"))
        path = CFG.getLkEnglishSecondary2Res();
      else if (CFG.getLang().contains("hi"))
        path = CFG.getLkEnglishSecondary3Res();
      else
        path = CFG.getLkEnglishSecondary1Res();

      final Resource lkEnglishSecondary = (Resource)EngineObject.load(engine, path);
      System.out.println(" . Secondary English LinguisticKnowledge resource loaded successfully");
      recognizer.attach(lkEnglishSecondary);
      System.out.println(" . Secondary English LinguisticKnowledge resource attached successfully");
      System.out.println(" . disposing of the secondary LinguisticKnowledge resource (not needed anymore)");
      lkEnglishSecondary.dispose();
      System.out.println(" . Secondary LinguisticKnowledge resource disposed of successfully");
    }
    System.out.println();
    
    System.out.println("4) creating a new FloatStructuredInput");
    final FloatStructuredInput input = FloatStructuredInput.create(engine);
    System.out.println(" . FloatStructuredInput created successfully");
    System.out.println();

    System.out.println("5) setting the input as the source of the recognizer");
    recognizer.setSource(input);
    System.out.println(" . input successfully set as the recognizer's source");
    System.out.println();
    
    System.out.println("6) processing input files");

    for (int i = 0; i < args.length; ++i)
    {
      if (!new File(args[i]).exists())
      {
        System.err.println("missing ink file: " + args[i]);
        System.exit(-1);
      }
      
      System.out.println(" . parsing: " + new File(args[i]).getCanonicalPath());
      parseInput(args[i], input, formField);
      System.out.println("   . input parsed successfully");
      System.out.println();

      System.out.println(" . running the handwriting recognition process");
      recognizer.run();
      System.out.println("   . handwriting recognition performed successfully");
      System.out.println();
      
      System.out.println(" . retrieving the recognition result");
      final RecognitionResult result = recognizer.getResult();
      System.out.println("   . RecognitionResult retrieved successfully");
      System.out.println();

      System.out.println(" . retrieving the first candidate of the RecognitionResult");
      final CandidateIterator iterator = result.getCandidates();
      if (!iterator.isAtEnd())
        System.out.println("   . input recognized as: \"" + iterator.getLabel() + '"');
      else
        System.out.println("   . sorry, unable to recognize the input");
      System.out.println();
      
      iterator.dispose();
      result.dispose();
      
      input.clear(false);
    }
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("7) cleaning-up: disposing of objects and disposing of the Engine");
    input.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform handwriting recognition on isolated digital ink");
  }

  private final static void parseInput(final String path, FloatStructuredInput input, FormField formField)
  {
    final Ink ink = (Ink)EngineObject.load(input.getEngine(), path);
    
    for (int j = 0; j < formField.rowCount; ++j)
    {
      for (int k = 0; k < formField.columnCount; ++k)
      {
        // create a rectangular selection
        final InkSelection selection = InkSelection.create(ink);

        // set the active box coordinates
        final Rectangle activeBox = new Rectangle();
        
        activeBox.x = formField.x0 + formField.cellWidth * k;
        activeBox.y = formField.y0 + formField.cellHeight * j;
        activeBox.width = formField.cellWidth;
        activeBox.height = formField.cellHeight;

        // select ink in a character field
        selection.selectRectangle(activeBox, SelectionModifier.SET);

        // make sure entire strokes are selected
        selection.adjustToStrokeBoundaries(0.5f);

        // check that we selected something
        final int count = selection.getStrokeCount();
        if (count == -1)
          throw new RuntimeException("empty selection");

        // start input unit and send ink
        input.startInputUnit(InputUnit.CHAR);
        
        // set the guide box from the active box, applying the padding
        final Rectangle guideBox = new Rectangle(activeBox.x, activeBox.y, activeBox.width, activeBox.height);
        
        guideBox.x += formField.padding;
        guideBox.y += formField.padding;
        guideBox.width -= 2.0f * formField.padding;
        guideBox.height -= 2.0f * formField.padding;
        
        input.setGuideBox(guideBox.x, guideBox.y, guideBox.width, guideBox.height);

        for (int i = 0; i < selection.getStrokeCount(); ++i)
        {
          final InkStroke stroke = selection.getStrokeAt(i);
          
          input.addStroke(stroke.getValuesAsFloat(0), stroke.getValuesAsFloat(1));
          
          stroke.dispose();
        } // end of: for (i = 0; i < selection.getStrokeCount(); ++i)

        // end input unit
        input.endInputUnit(InputUnit.CHAR);
        
      } // end of: for (k = 0; k < formField.columnCount; ++k)
      
    } // end of: for (j = 0; j < formField.rowCount; ++j)
    
    ink.dispose();
  }
  
} // IsolatedRecognition
