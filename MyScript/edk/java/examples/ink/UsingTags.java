package ink;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.geometry.Rectangle;
import com.myscript.ink.Ink;
import com.myscript.ink.InkSegmentMatchMode;
import com.myscript.ink.InkSelection;
import com.myscript.ink.InkTagIterator;
import com.myscript.ink.InkTagQuery;
import com.myscript.ink.SelectionModifier;
import com.myscript.json.Json;
import com.myscript.json.JsonObjectIterator;
import common.CFG;

/**
 * The <code>UsingTags/code> example code shows you how to use tags within ink.
 */
public class UsingTags
{
  /**
   * Path to the .jink file
   */
  private static final String JINK_FILE = CFG.DATA_DIR + "/flower.jink";

  /**
   * Main entry point for the <code>UsingTags</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (JINK_FILE == null || JINK_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for jink file. You can choose document filename by editing JINK_FILE static member variable of UsingTags class");
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
    final InkTagQuery tagQuery = new InkTagQuery();
    tagQuery.mode = InkSegmentMatchMode.MATCH_INCLUDE;
    tagQuery.intervals = null;
    int tagCount = ink.tagLookupCount(tagQuery);
    System.out.println(" . the Ink contains " + tagCount + " tags");
    System.out.println();

    System.out.println("4) selecting a portion of Ink");
    final InkSelection selection = InkSelection.create(ink);
    // select a portion of ink
    final Rectangle rectangle = new Rectangle(230, 300, 50, 50);
    selection.selectRectangle(rectangle, SelectionModifier.SET);
    // check that we selected something
    System.out.println(" . selection created with " + selection.getStrokeCount() + " strokes");
    System.out.println();

    System.out.println("5) adjusting the selection to tag boundaries");
    selection.adjustToTagBoundaries("chinese_word", 0.3f);
    System.out.println(" . the adjusted selection now contains " + selection.getStrokeCount() + " strokes");
    System.out.println();

    System.out.println("6) inspecting the tags intersecting with this selection");
    tagQuery.intervals = selection;
    tagQuery.mode = InkSegmentMatchMode.MATCH_INTERSECT;
    tagCount = ink.tagLookupCount(tagQuery);
    System.out.println(" . the adjusted selection intersects with " + tagCount + " tags:");
    InkTagIterator tagIterator = ink.tagLookup(tagQuery);
    while(false == tagIterator.isAtEnd())
    {
      System.out.println("     tag id = " + tagIterator.getId() + " , tag name = \"" + tagIterator.getName() + "\"");
      tagIterator.next();
    }
    tagIterator.dispose();
    System.out.println();

    tagQuery.intervals = null;
    tagQuery.id = new Long(1);
    System.out.println("7) selecting the tag with id " + tagQuery.id);
    tagIterator = ink.tagLookup(tagQuery);
    EngineObject tagData = null;
    if (!tagIterator.isAtEnd())
    {
      System.out.println(" . the name of this tag is \"" + tagIterator.getName() + "\"");
      tagData = tagIterator.getData();
      if (tagData != null)
      {
        if (tagData instanceof Json)
        {
          System.out.println(" . the tag has associated data of type JSON");
        }
      }
      tagIterator.next();
    }
    tagIterator.dispose();
    System.out.println();

    System.out.println("8) inspecting JSON data associated with tag " + tagQuery.id);
    final Json jsonObject = (Json)(tagData);
    final JsonObjectIterator jsonIterator = jsonObject.getObjectEntries();
    System.out.println(" . the JSON data is a JSON object which contains the following keys:");
    while(false == jsonIterator.isAtEnd())
    {
      System.out.println("     key = \"" + jsonIterator.getKey() + "\"");
      jsonIterator.next();
    }
    jsonIterator.dispose();
    tagData.dispose();
    System.out.println();

    System.out.println("9) erasing the selection");
    ink.erase(selection);
    tagQuery.id = new Long(-1);
    tagCount = ink.tagLookupCount(tagQuery);
    System.out.println(" . the Ink contains " + tagCount + " tags after a selection removal");
    System.out.println();
    
    tagQuery.id = new Long(1);
    System.out.println("10) erasing tag with id " + tagQuery.id);
    tagIterator = ink.tagLookup(tagQuery);
    if (!tagIterator.isAtEnd())
    {
      tagIterator.remove();
      tagIterator.next();
      tagIterator.dispose();
      tagQuery.id = new Long(-1);
      tagCount = ink.tagLookupCount(tagQuery);
      System.out.println(" . the Ink contains " + tagCount + " tags after tag removal");
    }
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("11) cleaning-up: disposing of objects and disposing of the Engine");
    selection.dispose();
    ink.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to use tags within ink");
  }

} // UsingTags
