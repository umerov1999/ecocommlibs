package music;

import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;

import certificates.MyCertificate;

import com.myscript.edk.java.music.ScoreToMusicXML;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.ink.Ink;
import com.myscript.ink.InkStroke;
import com.myscript.ink.InkTagIterator;
import com.myscript.ink.InkTagQuery;
import com.myscript.json.Json;
import com.myscript.music.MusicAlphabetKnowledge;
import com.myscript.music.MusicDocument;
import com.myscript.music.MusicGrammar;
import com.myscript.music.MusicRecognizer;
import com.myscript.music.MusicScore;
import common.CFG;

/**
 * The <code>MusicRecognition</code> example code shows you how to perform
 * recognition on a handwritten music document.
 */
public class MusicRecognition
{
  /**
   * Main entry point for the <code>MusicRecognition</code> example program.
   */
  public static final void main(String[] args) throws IOException
  {
    if (args.length == 0)
    {
      args = new String[1];
      args[0] = CFG.DATA_DIR + "/music.jink";
    }
    
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a new MusicRecognizer");
    final MusicRecognizer recognizer = MusicRecognizer.create(engine);
    System.out.println(" . MusicRecognizer created successfully");
    System.out.println();

    System.out.println("3) loading and attaching resources to the recognizer");
    final MusicAlphabetKnowledge ak = (MusicAlphabetKnowledge)EngineObject.load(engine, CFG.getMusicAkRes());
    System.out.println(" . MusicAlphabetKnowledge resource loaded successfully");
    recognizer.attach(ak);
    System.out.println(" . MusicAlphabetKnowledge resource attached successfully");
    System.out.println(" . disposing of the MusicAlphabetKnowledge resource (not needed anymore)");
    ak.dispose();
    System.out.println(" . MusicAlphabetKnowledge resource disposed of successfully");
    
    final MusicGrammar grm = (MusicGrammar)EngineObject.load(engine, CFG.getMusicGrmRes());
    System.out.println(" . MusicGrammar resource loaded successfully");
    recognizer.attach(grm);
    System.out.println(" . MusicGrammar resource attached successfully");
    System.out.println(" . disposing of the MusicGrammar resource (not needed anymore)");
    grm.dispose();
    System.out.println(" . MusicGrammar resource disposed of successfully");
    System.out.println();

    System.out.println("4) creating a new MusicDocument");
    final MusicDocument document = MusicDocument.create(engine);
    System.out.println(" . MusicDocument created successfully");
    System.out.println();
    
    System.out.println("5) processing input files");

    for (int i = 0; i < args.length; ++i)
    {
      if (!new File(args[i]).exists())
      {
        System.err.println("missing ink file: " + args[i]);
        System.exit(-1);
      }
      
      System.out.println(" . parsing: " + new File(args[i]).getCanonicalPath());
      parseInput(args[i], document);
      System.out.println("   . input parsed successfully");

      System.out.println(" . running the music recognition process");
      recognizer.process(document);
      System.out.println("   . music recognition performed successfully");
      
      System.out.println(" . retrieving the recognition result");
      final MusicScore score = document.getScore();
      if (score != null)
      {
        final String path = new File(CFG.OUTPUT_DIR + "/MusicRecognition-output.xml").getCanonicalPath();
        final PrintWriter out = new PrintWriter(path);
        out.print(ScoreToMusicXML.format(score));
        out.close();
        
        System.out.println("   . score stored as MusicXML: " + path);
        System.out.println();

        score.dispose();
      }
      else
      {
        System.out.println(" . no recognition result");
        System.out.println();
      }      
    }

    // do not forget the 1st rule: you create it, you dispose of it !
    System.out.println("6) cleaning-up: disposing of objects and disposing of the Engine");
    document.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform music recognition");
  }

  private final static void parseInput(final String path, MusicDocument document)
  {
    final Ink ink = (Ink)EngineObject.load(document.getEngine(), path);
    
    final InkTagQuery query = new InkTagQuery();
    query.name = "STAFF";
    final InkTagIterator tagIterator = ink.tagLookup(query);
    final Json data = (Json)tagIterator.getData();    
  
    Json value;
    
    value = data.getObjectEntryValue("count");
    final int count = (int)value.getNumberValue();
    value.dispose();
    
    value = data.getObjectEntryValue("top");
    final float top = (float)value.getNumberValue();
    value.dispose();
    
    value = data.getObjectEntryValue("gap");
    final float gap = (float)value.getNumberValue();
    value.dispose();
    
    document.addStaff(count, top, gap);

    for (int i = 0; i < ink.getStrokeCount(); ++i)
    {
      final InkStroke stroke = ink.getStrokeAt(i);
      
      document.addStroke(stroke.getValuesAsFloat(0), stroke.getValuesAsFloat(1));
      
      stroke.dispose();
    }
  
    data.dispose();
    tagIterator.dispose();
    ink.dispose();
  }
 
} // MusicRecognition
