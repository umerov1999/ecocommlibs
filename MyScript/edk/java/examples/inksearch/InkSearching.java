package inksearch;

import java.io.File;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.inksearch.FindResult;
import com.myscript.inksearch.Finder;
import com.myscript.inksearch.Index;
import com.myscript.inksearch.OccurrenceIterator;
import com.myscript.inksearch.OrQuery;
import com.myscript.inksearch.StringQuery;
import common.CFG;

/**
 * The <code>InkSearching</code> example code shows you how to perform ink
 * searching on digital ink that has already been indexed.
 */
public final class InkSearching
{
  /**
   * The file where the index will be stored.
   */
  private static final String INDEX_FILE = CFG.OUTPUT_DIR + "/InkIndexing-output.index";

  /**
   * Main entry point for the <code>InkSearching</code> example program.
   *
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args)
  {
    if (INDEX_FILE == null || INDEX_FILE.length() == 0)
    {
      System.err.println("/!\\ please choose a valid filename for index file. You can choose index filename by editing INDEX_FILE static member variable of InkSearching class");
      System.exit(-1);
    }

    if (!(new File(INDEX_FILE)).exists())
    {
      System.err.println("/!\\ please run StructuredInputInkIndexing first in order to generate the index file which is needed to run this example");
      System.exit(-1);
    }

    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a BinaryQuery (\"Vision OR Objects\")");
    final StringQuery query1 = StringQuery.create(engine, "Vision", true, true, false);
    System.out.println(" . \"Vision\" StringQuery created successfully");
    final StringQuery query2 = StringQuery.create(engine, "Objects", true, true, false);
    System.out.println(" . \"Objects\" StringQuery created successfully");
    final OrQuery query = OrQuery.create(engine, query1, query2);
    System.out.println(" . \"Vision OR Objects\" OrQuery created successfully");
    System.out.println();

    System.out.println("3) creating a Finder");
    final Finder finder = Finder.create(engine);
    System.out.println(" . Finder created successfully");
    System.out.println();

    System.out.println("4) attaching the OrQuery to the Finder");
    finder.attach(query);
    System.out.println(" . OrQuery attached successfully");
    System.out.println();

    System.out.println("5) loading the Index generated from indexing example");
    Index index = (Index)EngineObject.load(engine, InkSearching.INDEX_FILE);
    System.out.println(" . Index loaded successfully");
    System.out.println();

    System.out.println("6) attaching the Index to the Finder");
    finder.attach(index);
    System.out.println(" . Index attached successfully");
    System.out.println();

    System.out.println("7) running the search process");    
    finder.run();
    System.out.println(" . search process performed successfully");
    System.out.println();

    System.out.println("8) retrieving the FindResult");
    final FindResult result = finder.getResult();
    System.out.println(" . FindResult retrieved successfully");
    System.out.println();

    System.out.println("9) displaying the result");
    System.out.println(" . global score: " + result.getGlobalScore());
    final OccurrenceIterator occurrences = result.getOccurrences();
    for (int i = 1; !occurrences.isAtEnd(); ++i)
    {
      System.out.println(" . Occurrence " + i +
                         ": score=" + occurrences.getScore() +
                         ", inputRange=" + occurrences.getInputRange() +
                         ", searchUnit=" + occurrences.getSearchUnit() );

      occurrences.next();
    }
    System.out.println();

    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("10) cleaning-up: disposing of objects and disposing of the Engine");
    occurrences.dispose();
    finder.dispose();
    query.dispose();
    query2.dispose();
    query1.dispose();
    index.dispose();
    result.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();

    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform ink searching");
  }

} // InkSearching
