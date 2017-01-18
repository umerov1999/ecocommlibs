package text;

import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.InputStreamReader;
import java.text.NumberFormat;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.engine.FloatStructuredInput;
import com.myscript.engine.Iterator;
import com.myscript.ink.Ink;
import com.myscript.ink.InkStroke;
import com.myscript.text.AccentuationScheme;
import com.myscript.text.CandidateIterator;
import com.myscript.text.CasingScheme;
import com.myscript.text.LexicalUnit;
import com.myscript.text.RecognitionResult;
import com.myscript.text.Recognizer;
import com.myscript.text.Resource;
import com.myscript.text.SegmentIterator;
import com.myscript.text.StructuredInputRecognizer;
import common.CFG;

/**
 * The <code>ExploreRecognitionResult</code> example code shows you how to
 * explore the <code>RecognitionResult</code> and how to manipulate it.
 */
public class ExploreRecognitionResult
{
  /**
   * Main entry point for the <code>ExploreRecognitionResult</code> example
   * program.
   *
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args) throws IOException
  {
    if (args.length == 0)
    {
      args = new String[1];
      args[0] = CFG.DATA_DIR + "/horsemanship.ink";
    }
    
    final Engine engine = Engine.create(MyCertificate.getBytes());

    final StructuredInputRecognizer recognizer = StructuredInputRecognizer.create(engine);

    final Resource ak = (Resource)EngineObject.load(engine, CFG.getAkRes());
    recognizer.attach(ak);
    ak.dispose();
    
    LexicalUnit lexicalUnit = LexicalUnit.create(engine);
    lexicalUnit.defineRegex("lexicon", "horse|house|man|more|ship|skip|horseman|warship|horsemanship");
    lexicalUnit.defineMainRegex("{lexicon}( {lexicon})*");
    
    lexicalUnit.compile();
    recognizer.attach(lexicalUnit);
    lexicalUnit.dispose();
    
    final FloatStructuredInput input = FloatStructuredInput.create(engine);
    recognizer.setSource(input);

    for (int i = 0; i < args.length; ++i)
    {
      if (!new File(args[i]).exists())
      {
        System.err.println("missing ink file: " + args[i]);
        System.exit(-1);
      }
      
      System.out.println("parsing: " + new File(args[i]).getCanonicalPath());
      parseInput(args[i], input);
      System.out.println();
      System.out.println();
      
      System.out.println("-- recognizer configuration ----------------------------------------------------");
      configureCandidateListProduction(recognizer);
      configureCasingAndAccentuationSchemeOverrides(recognizer);
      
      recognizer.run();
      
      final RecognitionResult result = recognizer.getResult();

      final CandidateIterator candidateIterator = result.getCandidates();
      
      if (!candidateIterator.isAtEnd())
        exploreRecognitionresult(result);
      else
        System.out.println("no recognition result found :(");
      
      candidateIterator.dispose();
      result.dispose();
      
      input.clear(false);
    }
    
    input.dispose();
    recognizer.dispose();
    engine.dispose();

    System.out.println();
    System.out.println("end of example:");
    System.out.println(" you just learnt how to explore an handwriting RecognitionResult");
  }

  /**
   * Configures the given recognizer so that it produces the desired number of
   * candidates at each level and the desired case variation constraints.
   * This has to be done before the recognition process.
   */
  private static final void configureCandidateListProduction(final Recognizer recognizer)
  {
    assert (recognizer != null) : "invalid recognizer: null is not allowed";
  
    recognizer.setCharacterCandidateListSize(5);
    recognizer.setWordCandidateListSize(5);
    recognizer.setTextCandidateListSize(10);
    recognizer.setDiscardCaseVariations(true);
  }
  
  /**
   * Configures the given recognizer so that it overrides the casing scheme and
   * accentuation scheme of the <code>Vocabulary</code> resources attached to
   * it.
   * This has to be done before the recognition process.
   */
  private static final void configureCasingAndAccentuationSchemeOverrides(final Recognizer recognizer) throws IOException
  {
    int value = -1;
    
    System.out.println("1) casing scheme override configuration");
    System.out.println(" [1] NONE");
    System.out.println(" [2] STRICT_CASE");
    System.out.println(" [3] LEGACY_CASE");
    System.out.println(" [4] LAX_CASE");
    System.out.println();
    
    value = displayMenu("choose a casing scheme override for the rest of the example", 4);
    System.out.println();
    
    switch(value)
    {
      case 3:
        recognizer.setOverrideCasingScheme(CasingScheme.LAX_CASE);
        break;
        
      case 2:
        recognizer.setOverrideCasingScheme(CasingScheme.LEGACY_CASE);
        break;
        
      case 1:
        recognizer.setOverrideCasingScheme(CasingScheme.STRICT_CASE);
        break;
        
      default:
        break;
    }

    System.out.println("2) accentuation scheme override configuration");
    System.out.println(" [1] NONE");
    System.out.println(" [2] STRICT_ACCENTUATION");
    System.out.println(" [3] LAX_ACCENTUATION");
    System.out.println();
    
    value = displayMenu("choose a casing scheme override for the rest of the example", 3);
    System.out.println();
    
    switch(value)
    {
      case 2:
        recognizer.setOverrideAccentuationScheme(AccentuationScheme.LAX_ACCENTUATION);
        break;
        
      case 1:
        recognizer.setOverrideAccentuationScheme(AccentuationScheme.STRICT_ACCENTUATION);
        break;
        
      default:
        break;
    }
  }
  
  /**
   * Shows how to explore the recognition result.
   */
  private static final void exploreRecognitionresult(final RecognitionResult result) throws IOException
  {
    assert (result != null) : "invalid recognition resutl: null is not allowed";

    System.out.println("-- recognition result structure analysis ---------------------------------------");

    final NumberFormat nf = NumberFormat.getInstance();
    nf.setMinimumIntegerDigits(2);
    
    System.out.println("1) candidate list at the text level");
    CandidateIterator textCandidates = result.getCandidates();
    int textCandidateCount = printCandidateLabels(textCandidates, -1);
    textCandidates.dispose();
    System.out.println();

    int textCandidateIndex = displayMenu("2) choose a text candidate for the rest of the example", textCandidateCount);
    CandidateIterator textCandidate = getTextCandidate(result, textCandidateIndex);
    System.out.print(" [");
    System.out.print(nf.format(textCandidateIndex + 1));
    System.out.print("] \"");
    System.out.print(textCandidate.getLabel());
    System.out.println("\" chosen");
    System.out.println();

    System.out.println("3) selected candidates for each word segment of the chosen text candidate");
    System.out.println("   see the documentation for the concept of \"selected candidate\"");
    SegmentIterator wordSegments = textCandidate.getSegments();
    int wordSegmentCount = printSelectedCandidateLabels(wordSegments, true);
    wordSegments.dispose();
    System.out.println();

    int wordSegmentIndex =displayMenu("4) choose a word segment for the rest of the example", wordSegmentCount);
    wordSegments = textCandidate.getSegments();
    String label = getSegmentSelectedCandidateLabel(wordSegments, wordSegmentIndex, true);
    System.out.print(" [");
    System.out.print(nf.format(wordSegmentIndex + 1));
    System.out.print("] \"");
    System.out.print(label);
    System.out.println("\" chosen");
    wordSegments.dispose();
    System.out.println();

    System.out.println("5) candidate list for the chosen word segment");
    SegmentIterator wordSegment = getSegment(textCandidate, wordSegmentIndex, true);
    int selectedWordCandidateIndex = wordSegment.getSelectedCandidateIndex();
    CandidateIterator wordCandidates = wordSegment.getCandidates();
    int wordCandidateCount = printCandidateLabels(wordCandidates, selectedWordCandidateIndex);
    wordCandidates.dispose();
    wordSegment.dispose();
    System.out.println();
    
    int wordCandidateIndex = displayMenu("6) choose a word candidate for the rest of the example", wordCandidateCount);
    wordSegments = textCandidate.getSegments();
    CandidateIterator wordCandidate = getSegmentCandidate(wordSegments,
                                                          wordSegmentIndex,
                                                          wordCandidateIndex,
                                                          true);
    System.out.print(" [");
    System.out.print(nf.format(wordSegmentIndex + 1));
    System.out.print("] \"");
    System.out.print(wordCandidate.getLabel());
    System.out.println("\" chosen");
    wordSegments.dispose();
    System.out.println();

    System.out.println("7) selected candidates for each character segment of the chosen word candidate");
    System.out.println("   see the documentation for the concept of \"selected candidate\"");
    SegmentIterator characterSegments = wordCandidate.getSegments();
    int characterSegmentCount = printSelectedCandidateLabels( characterSegments,
                                                              true);
    characterSegments.dispose();
    System.out.println();

    int characterSegmentIndex = displayMenu("8) choose a character segment for the rest of the example", characterSegmentCount);
    characterSegments = wordCandidate.getSegments();
    label = getSegmentSelectedCandidateLabel( characterSegments,
                                              characterSegmentIndex,
                                              false);
    System.out.print(" [");
    System.out.print(nf.format(characterSegmentIndex + 1));
    System.out.print("] \"");
    System.out.print(label);
    System.out.println("\" chosen");
    wordSegments.dispose();
    System.out.println();
    
    System.out.println("9) candidate list for the chosen character segment");
    SegmentIterator characterSegment = getSegment(wordCandidate, characterSegmentIndex, true);
    int selectedCharacterCandidateIndex = characterSegment.getSelectedCandidateIndex();
    CandidateIterator characterCandidates = characterSegment.getCandidates();
    printCandidateLabels(characterCandidates, selectedCharacterCandidateIndex);
    characterCandidates.dispose();
    characterSegment.dispose();
    System.out.println();

    textCandidate.dispose();
    wordCandidate.dispose();

    System.out.println("-- recognition result scores analysis ------------------------------------------");

    System.out.println("format: NRS: (normalized recognition score) - (label)\n");

    System.out.println("1) candidate list at the text level");
    textCandidates = result.getCandidates();
    textCandidateCount = printCandidateLabels(textCandidates, -1);
    textCandidates.dispose();
    System.out.println();

    textCandidateIndex = displayMenu("2) choose a text candidate for the rest of the example", textCandidateCount);
    textCandidate = getTextCandidate(result, textCandidateIndex);
    System.out.print(" [");
    System.out.print(nf.format(textCandidateIndex + 1));
    System.out.print("] \"");
    System.out.print(textCandidate.getLabel());
    System.out.println("\" chosen");
    System.out.println();

    System.out.println("3) chosen text candidate");

    nf.setMinimumIntegerDigits(1);
    nf.setMinimumFractionDigits(4);
    nf.setMaximumFractionDigits(4);
    
    System.out.print(" NRS: ");
    System.out.print(nf.format(textCandidate.getNormalizedRecognitionScore()));
    System.out.print(" - ");
    System.out.print("\"");
    System.out.print(textCandidate.getLabel());
    System.out.println("\"");

    System.out.println();

    System.out.println("4) selected candidates for each word segment of the chosen text candidate");
    wordSegments = textCandidate.getSegments();
    printSelectedCandidateScores(wordSegments);
    wordSegments.dispose();
    System.out.println();

    System.out.println("5) selected candidates for each character segment of the chosen text candidate");
    wordSegments = textCandidate.getSegments();
    while (!wordSegments.isAtEnd())
    {
      wordCandidate = getSelectedCandidate(wordSegments);
      
      characterSegments = wordCandidate.getSegments();
      printSelectedCandidateScores(characterSegments);
      characterSegments.dispose();
      
      wordSegments.next();
    }
    wordSegments.dispose();

    textCandidate.dispose();
    wordCandidate.dispose();
  }
    
  /**
   * Prints the label of each candidate of the candidate iterator.
   * 
   * @param iterator a <code>CandidateIterator</code> object.
   * @param selected the index of the selected candidate.
   * 
   * @return the number of candidates printed.
   */
  private static final int printCandidateLabels(final CandidateIterator iterator,
                                                final int selected)
  {
    assert (iterator != null) : "invalid iterator: null is not allowed";
    assert (!iterator.isAtEnd()) : "invalid iterator: already at end";
    
    int i = 0;
    
    final NumberFormat nf = NumberFormat.getInstance();
    nf.setMinimumIntegerDigits(2);
    
    while (!iterator.isAtEnd())
    {
      System.out.print(" [");
      System.out.print(nf.format(i+1));
      System.out.print("] \"");
      System.out.print(iterator.getLabel());
      System.out.print("\"");
      
      if (selected != -1 && i == selected)
        System.out.print(" (selected)");

      System.out.println();
      
      ++i;
      iterator.next();
    }
    
    return i;
  }
  
  /**
   * Displays a menu for user selection.
   *
   * @param message a message to be displayed.
   * @param count the number of values that can be chosen.
   *
   * @return the user choice.
   */
  private static final int displayMenu(final String message, int count) throws IOException
  {
    final BufferedReader reader = new BufferedReader(new InputStreamReader(System.in));
    int menuItem = 0;
    
    do
    {
      if (message != null)
        System.out.print(message);
      
      System.out.print(" [1-");
      System.out.print(count);
      System.out.print("]: ");
      
      try
      {
        menuItem = Integer.parseInt(reader.readLine());
      }
      catch(final NumberFormatException exception)
      {
        // dismiss exception
      }
    }
    while (menuItem == 0 || menuItem > count); // loop until a correct value is entered
    
    return menuItem - 1;
  }
  
  /**
   * Moves the given iterator by <code>count</code> steps.
   * 
   * @param iterator an <code>Iterator</code> object.
   * @param count the number of steps to perform.
   * 
   * @return The number of steps actually performed.
   */
  private static final int moveIterator(final Iterator iterator,
                                        final int count)
  {
    assert (iterator != null) : "invalid iterator: null is not allowed";
    assert (count >= 0) : "invalid count: must be a positive value";
    
    int i;
    
    for (i = 0; i < count && !iterator.isAtEnd(); ++i)
      iterator.next();
    
    return i;
  }
  
  /**
   * Returns a candidate iterator positioned on the text candidate at the
   * specified <code>index</code>.
   *
   * @param result a <code>RecognitionResult</code> object.
   * @param index the index of the text candidate to be returned.
   *
   * @return a <code>CandidateIterator</code> object positioned on the text
   * candidate at the specified <code>index</code>.
   */
  private static final CandidateIterator getTextCandidate(final RecognitionResult result,
                                                          final int index)
  {
    assert (result != null) : "invalid recognition result: null is not allowed";
    assert (index >= 0) : "invalid index: should be a positive value";
    
    final CandidateIterator candidateIterator = result.getCandidates();
    moveIterator(candidateIterator, index);
    
    return candidateIterator;
  }
  
  /**
   * Returns a candidate iterator positioned on the selected candidate of the
   * specified segment.
   *
   * @param segment a <code>SegmentIterator</code> object.
   *
   * @return a <code>CandidateIterator</code> object positioned on the selected
   * candidate of the specified segment.
   */
  private static final CandidateIterator getSelectedCandidate(final SegmentIterator segment)
  {
    assert (segment != null) : "invalid segment: null is not allowed";

    final CandidateIterator candidate = segment.getCandidates();
    moveIterator(candidate, segment.getSelectedCandidateIndex());
    
    return candidate;
  }
  
  /**
   * Tells if the specified label is a space.
   *
   * @return <code>true</code> if the specified label is a space, otherwise
   * <code>false</code>.
   */
  private static final boolean isSpace(final String label)
  {
    assert (label != null) : "invalid label: null is not allowed";

    return label.equals("") || label.equals(" ") || label.equals("\u00A0");
  }
  
  /**
   * Prints the label of the selected candidate for each segment.
   *
   * @param segments a <code>SegmentIterator</code> object.
   * @param ignoreSpaceSegments ignore space segments.
   *
   * @return the number of candidate printed.
   */
  private static final int printSelectedCandidateLabels(final SegmentIterator segments,
                                                        final boolean ignoreSpaceSegments)
  {
    assert (segments != null) : "invalid segment iterator";
    assert (!segments.isAtEnd()) : "iterator is at end";

    int current = 1;
    
    final NumberFormat nf = NumberFormat.getInstance();
    nf.setMinimumIntegerDigits(2);

    while (!segments.isAtEnd())
    {
      final CandidateIterator candidate = getSelectedCandidate(segments);

      final String label = candidate.getLabel();

      if (ignoreSpaceSegments && isSpace(label))
        System.out.print(" [--] ");
      else
      {
        System.out.print(" [");
        System.out.print(nf.format(current++));
        System.out.print("] ");
      }

      System.out.print("\"");
      System.out.print(label);
      System.out.println("\"");

      candidate.dispose();

      segments.next();
    }

    return current - 1;
  }
  
  /**
   * Returns the label of the selected candidate for the specified segment.
   *
   * @param segments a <code>SegmentIterator</code> positioned at the beginning
   * of the iteration.
   * @param index the index of the segment.
   * @param ignoreSpaceSegments ignore space segments.
   *
   * @return the label of the selected candidate for the specified segment.
   */
  private static final String getSegmentSelectedCandidateLabel( final SegmentIterator segments,
                                                                final int index,
                                                                final boolean ignoreSpaceSegments)
  {
    int current = 0;

    assert (segments != null) : "invalid segment iterator";
    assert (index >= 0) : "invalid index";
    assert(!segments.isAtEnd()) : "iterator is at end";

    while (!segments.isAtEnd())
    {
      final CandidateIterator candidate = getSelectedCandidate(segments);

      final String label = candidate.getLabel();

      if (!(ignoreSpaceSegments && isSpace(label)) && (current++ == index))
        return label;

      segments.next();
    }

    return null;
  }  
  
  /**
   * Returns the specified candidate for the specified segment.
   *
   * @param segments a <code>SegmentIterator</code> positioned at the beginning
   * of the iteration.
   * @param index the index of the segment.
   * @param index the index of the candidate.
   * @param ignoreSpaceSegments ignore space segments.
   *
   * @return the specified candidate for the specified segment.
   */
  private static final CandidateIterator getSegmentCandidate( final SegmentIterator segments,
                                                              final int segmentIndex,
                                                              final int candidateIndex,
                                                              final boolean ignoreSpaceSegments)
  {
    int current = 0;

    assert(segments != null) : "invalid segment iterator";
    assert(segmentIndex >= 0) : "invalid segment index";
    assert(candidateIndex >= 0) : "invalid segment index";
    assert(!segments.isAtEnd()) : "iterator is at end";

    while (!segments.isAtEnd())
    {
      CandidateIterator candidate = getSelectedCandidate(segments);
      String label = candidate.getLabel();

      if (!(ignoreSpaceSegments && isSpace(label)) && (current++ == segmentIndex))
      {
        candidate = segments.getCandidates();
        moveIterator(candidate, candidateIndex);

        return candidate;
      }

      segments.next();
    }

    return null;
  }
  
  /**
   * Returns a <code>SegmentIterator</code> object positioned on the word
   * segment at the specified index of the given text candidate.
   *
   * @return a <code>SegmentIterator</code> object positioned on the word
   * segment at the specified index of the given text candidate.
   */
  private static final SegmentIterator getSegment(final CandidateIterator textCandidate,
                                                  final int index,
                                                  final boolean ignoreSpaceSegments)
  {
    assert (textCandidate != null) : "invalid text candidate";
    assert (!textCandidate.isAtEnd()) :"iterator is at end";
    assert (index >= 0) : "invalid index";

    final SegmentIterator segment = textCandidate.getSegments();
    int current = 0;
    
    while (!segment.isAtEnd())
    {
      final CandidateIterator candidate = getSelectedCandidate(segment);

      final String label = candidate.getLabel();

      if (!(ignoreSpaceSegments && isSpace(label)) && (current++ == index))
        return segment;

      segment.next();
    }

    return segment;
  }  
  
  /**
   * Prints the label of the selected candidate for each segment.
   *
   * @param segments a <code>SegmentIterator</code> object.
   *
   * @return the number of candidate printed.
   */
  private static final void printSelectedCandidateScores(final SegmentIterator segments)
  {
    assert (segments != null) : "invalid segment iterator";
    assert (!segments.isAtEnd()) : "iterator is at end";

    while (!segments.isAtEnd())
    {
      final CandidateIterator candidate = getSelectedCandidate(segments);

      final NumberFormat nf = NumberFormat.getInstance();
      nf.setMinimumFractionDigits(4);
      nf.setMaximumFractionDigits(4);
      
      System.out.print(" NRS: ");
      System.out.print(nf.format(candidate.getNormalizedRecognitionScore()));
      System.out.print(" - ");
      System.out.print("\"");
      System.out.print(candidate.getLabel());
      System.out.println("\"");

      candidate.dispose();

      segments.next();
    }
  }
  
  private final static void parseInput(final String path, FloatStructuredInput input)
  {
    final Ink ink = (Ink)EngineObject.load(input.getEngine(), path);
    
    for (int i = 0; i < ink.getStrokeCount(); ++i)
    {
      final InkStroke stroke = ink.getStrokeAt(i);
      
      input.addStroke(stroke.getValuesAsFloat(0), stroke.getValuesAsFloat(1));
      
      stroke.dispose();
    }
  
    ink.dispose();
  }

} // ExploreRecognitionResult
