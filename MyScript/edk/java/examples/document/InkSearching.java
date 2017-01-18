package document;

import java.io.File;
import java.io.IOException;

import certificates.MyCertificate;

import com.myscript.document.Document;
import com.myscript.document.DocumentFindOccurrenceIterator;
import com.myscript.document.DocumentFindResult;
import com.myscript.document.DocumentFinder;
import com.myscript.document.IDocumentFinderHandler;
import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.inksearch.OrQuery;
import com.myscript.inksearch.StringQuery;
import common.CFG;

/**
 * The <code>InkSearching</code> example code shows you how to search into a
 * previously indexed document.
 */
public class InkSearching
{
  /**
   * The temporary directory used in this example.
   */
  private static final String TEMPORARY_DIRECTORY = CFG.OUTPUT_DIR + "/tmp";
  
  /**
   * The document file opened to load its index. (Note that you can rename any
   * notes document as a zip file to watch inside it.) Parts of the document
   * must have been previously indexed, please look at the project
   * "document/TextIndexation". ("examples3.notes" is "examples2.notes"
   * with indexation data)
   */
  private static final String DOCUMENT_FILE = CFG.DATA_DIR + "/example3.notes";

  /**
   * Helper function to display results
   */
  private static final void printOccurrence(DocumentFindOccurrenceIterator occurrenceIterator)
  {
    String pageId = occurrenceIterator.getPageId();
    String inkSegment = occurrenceIterator.getInkSegment();
    String label = occurrenceIterator.getLabel();
    float score = occurrenceIterator.getScore();
    int queryIndex = occurrenceIterator.getQueryIndex();
    System.out.println("found \"" + label + "\" (sub query " + queryIndex + "), at page \"" + pageId + "\" - \""
        + inkSegment + "\", with score " + score);
  }

  /**
   * Main entry point for the <code>InkSearching</code> example program.
   * 
   * @param args command line arguments, ignored.
   * @throws IOException
   */
  public static final void main(String[] args) throws IOException
  {
    if (DOCUMENT_FILE == null || DOCUMENT_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for document file. You can choose document filename by editing DOCUMENT_FILE static member variable of InkSearching class");
      System.exit(-1);
    }
    
    if (TEMPORARY_DIRECTORY == null || TEMPORARY_DIRECTORY.length() == 0)
    {
      System.err.println("/!\\ please choose a valid path for document temporary directory. You can choose the path by editing TEMPORARY_DIRECTORY static member variable of InkSearching class");
      System.exit(-1);
    }

    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) loading an existing Document containing indexed texts");
    final Document document = (Document)EngineObject.load(engine, DOCUMENT_FILE);
    document.setTemporaryDirectory(TEMPORARY_DIRECTORY);
    System.out.println(" . " + new File(DOCUMENT_FILE).getCanonicalPath() + " loaded successfully");
    System.out.println();

    System.out.println("3) creating a complex search query");
    final StringQuery subQuery1 = StringQuery.create(engine, "wish*", true, true, true);
    System.out.println(" . first subquery created successfully");

    final StringQuery subQuery2 = StringQuery.create(engine, "birthday", true, true, false);
    System.out.println(" . second subquery created successfully");

    final OrQuery query = OrQuery.create(engine, subQuery1, subQuery2);
    System.out.println(" . complex query created successfully");
    System.out.println("");

    System.out.println("4) create a document finder");
    final DocumentFinder finder = DocumentFinder.create(document, new IDocumentFinderHandler()
    {
      public void onStart()
      {
        System.out.println("  > Find process started...");
      }
      
      public void onNewOccurrences(DocumentFindOccurrenceIterator occurrenceIterator)
      {
        System.out.println("  > New occurrence(s) found:");
        int index = 0;
        while (!occurrenceIterator.isAtEnd())
        {
          System.out.print("    #" + index + " -- ");
          printOccurrence(occurrenceIterator);
          occurrenceIterator.next();
          ++index;
        }
      }

      public void onDone()
      {
        System.out.println("  > Find process done");

      }

      public void onCancelled()
      {
        System.out.println("  > Find process cancelled");
      }
    });
    System.out.println(" . document finder created successfully");
    System.out.println("");

    System.out.println("5) launch the find process (look for 'wish*' and 'birthday') & wait for the result");
    finder.process(query);
    final DocumentFindResult result = finder.getResult(true);
    System.out.println("");

    System.out.println("6) inspect the result");
    result.getOccurrences();
    final DocumentFindOccurrenceIterator occurrenceIterator = result.getOccurrences();

    int index = 0;
    while (!occurrenceIterator.isAtEnd())
    {
      System.out.print(" . occurence #" + index + " -- ");
      printOccurrence(occurrenceIterator);
      occurrenceIterator.next();
      ++index;
    }
    occurrenceIterator.dispose();
    System.out.println("");

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("7) cleaning-up: disposing of objects and disposing of the Engine");
    result.dispose();
    subQuery2.dispose();
    subQuery1.dispose();
    query.dispose();
    document.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println("");

    System.out.println("end of example:");
    System.out.println(" you just learnt how to search into an indexed document");
  }

} // InkSearching
