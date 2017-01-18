package text;

import certificates.MyCertificate;

import com.myscript.engine.Engine;
import com.myscript.engine.EngineObject;
import com.myscript.engine.FloatStructuredInput;
import com.myscript.text.CandidateIterator;
import com.myscript.text.DigitalInkTagger;
import com.myscript.text.LexicalUnit;
import com.myscript.text.RecognitionResult;
import com.myscript.text.Resource;
import com.myscript.text.StructuredInputRecognizer;
import com.myscript.text.TagIterator;
import com.myscript.text.TagList;
import common.CFG;

/**
 * The <code>DigitalInkTaggingWithRecognition</code> example code shows you how
 * to perform digital ink tagging with conjoint recognition.
 */
public class DigitalInkTaggingWithRecognition
{
  /**
   * Main entry point for the <code>DigitalInkTaggingWithRecognition</code> 
   * example program.
   *
   * @param args command line arguments, ignored.
   */
  public static final void main(String[] args)
  {
    System.out.println("1) creating a new Engine initialized with your certificate");
    final Engine engine = Engine.create(MyCertificate.getBytes());
    System.out.println(" . Engine created successfully");
    System.out.println();

    System.out.println("2) creating a new StructuredInputRecognizer");
    final StructuredInputRecognizer recognizer = StructuredInputRecognizer.create(engine);
    System.out.println(" . StructuredInputRecognizer created successfully");
    System.out.println();

    System.out.println("3) loading and attaching resources to the recognizer");
    System.out.println(" . loading the AlphabetKnowledge resource");
    final Resource ak = (Resource)EngineObject.load(engine, CFG.getAkRes());
    System.out.println(" . AlphabetKnowledge resource loaded successfully");

    System.out.println(" . attaching the AlphabetKnowledge resource to the recognizer");
    recognizer.attach(ak);
    System.out.println(" . AlphabetKnowledge resource attached successfully");
    
    System.out.println(" . disposing of the AlphabetKnowledge resource (not needed anymore)");
    ak.dispose();
    System.out.println(" . AlphabetKnowledge resource disposed of successfully");
    
    System.out.println(" . creating a LexicalUnit resource used to constrain the recognition");
    final LexicalUnit lu = LexicalUnit.create(engine);
    lu.defineRegex("lexicon", "hello|joe|john|how|are|you");
    lu.defineRegex("punct", "?");
    lu.defineMainRegex("{lexicon}( {lexicon}|{punct})*");

    System.out.println(" . compiling the LexicalUnit resource");
    lu.compile();
    System.out.println(" . LexicalUnit resource compiled successfully");
    System.out.println(" . attaching the LexicalUnit resource to the recognizer");
    recognizer.attach(lu);
    System.out.println(" . LexicalUnit resource attached successfully");
    
    System.out.println(" . disposing of the LexicalUnit resource (not needed anymore)");
    lu.dispose();
    System.out.println(" . LexicalUnit resource disposed of successfully");
    System.out.println();
    
    System.out.println("4) creating a new DigitalInkTagger");
    final DigitalInkTagger tagger = DigitalInkTagger.create(engine);
    System.out.println(" . DigitalInkTagger created successfully");
    System.out.println();

    System.out.println("5) attaching the tagger to the recognizer");
    recognizer.attach(tagger);
    System.out.println(" . DigitalInkTagger attached successfully");
    System.out.println();
    
    System.out.println("6) creating a new FloatStructuredInput");
    final FloatStructuredInput input = FloatStructuredInput.create(engine);
    System.out.println(" . FloatStructuredInput created successfully");
    System.out.println();

    System.out.println("7) fueling the input with strokes from the digital ink");

    // for each stroke
    for (int i = 0; i < DIGITAL_INK.length; ++i)
    {
      input.addStroke(DIGITAL_INK[i], 0, 2,       // x coordinates
                      DIGITAL_INK[i], 1, 2,       // y coordinates
                      DIGITAL_INK[i].length / 2); // point count
    }
    
    System.out.println(" . input fueled successfully");
    System.out.println();
    
    System.out.println("8) setting the input as the source of the recognizer");
    recognizer.setSource(input);
    System.out.println(" . input successfully set as the recognizer's source");
    System.out.println();
    
    System.out.println("9) running the conjoint digital ink tagging with recognition process");
    recognizer.run();
    System.out.println(" . conjoint digital ink tagging with recognition performed successfully");
    System.out.println();
    
    System.out.println("10) retrieving the TagList");
    TagList tags = tagger.getTags();
    System.out.println(" . TagList retrieved successfully");
    System.out.println();
    
    System.out.println("11) iterating over the tags in the TagList");
    
    TagIterator tagIterator = tags.getIterator();
    
    while(!tagIterator.isAtEnd())
    {
      System.out.println( " . " + tagIterator.getType().getName() +
                          ", " + tagIterator.getInputRange());

      tagIterator.next();
    }
    
    System.out.println();
    System.out.println("12) retrieving the RecognitionResult");
    final RecognitionResult result = recognizer.getResult();
    System.out.println(" . RecognitionResult retrieved successfully");
    System.out.println();
    
    System.out.println("13) retrieving the first candidate of the RecognitionResult");
    final CandidateIterator candidateIterator = result.getCandidates();
    if (!candidateIterator.isAtEnd())
      System.out.println(" . input recognized as: \"" + candidateIterator.getLabel() + '"');
    else
      System.out.println(" . sorry, unable to recognize the input");
    System.out.println();
    
    // do not forget the 1st rule: you create it, you dispose of it!
    System.out.println("14) cleaning-up: disposing of objects and disposing of the Engine");
    candidateIterator.dispose();
    result.dispose();
    tagIterator.dispose();
    tags.dispose();
    tagger.dispose();
    input.dispose();
    recognizer.dispose();
    engine.dispose();
    System.out.println(" . clean-up done successfully");
    System.out.println();
    
    System.out.println("end of example:");
    System.out.println(" you just learnt how to perform digital ink tagging with recognition");
  }

  /**
   * We did want to emphasize the usage of MyScript SDK, not how to load ink
   * from a file. This is why we hard coded the stroke data.
   * 
   * You can vizualize this ink by opening the
   * edk/examples/ink/DigitalInkTaggingWithRecognition.itf file.
   * 
   * <ul>
   * <li><code>DIGITAL_INK.length</code> gives you the number of strokes</li>
   * <li><code>DIGITAL_INK[0]</code> gives you access to the first stroke</li>
   * </ul>Stroke coordinates are interleaved: <code>x1, y1, x2, y2,</code> etc. 
   */
  private static float[][] DIGITAL_INK =
  {
    // stroke 0
    {
      268, 93,
      268, 94,
      267, 96,
      267, 99,
      267, 102,
      266, 107,
      266, 112,
      264, 119,
      264, 125,
      263, 132,
      263, 139,
      262, 145,
      262, 151,
      261, 156,
      261, 160,
      261, 163,
      261, 165,
      261, 166,
    },
    
    // stroke 1
    {
      310, 103,
      310, 107,
      310, 110,
      310, 114,
      310, 119,
      309, 124,
      309, 130,
      309, 135,
      309, 140,
      309, 145,
      309, 149,
      309, 153,
      309, 155,
      308, 157
    },
    
    // stroke 2
    {
      251, 144,
      252, 145,
      254, 146,
      257, 146,
      260, 146,
      265, 146,
      271, 146,
      278, 145,
      285, 144,
      292, 143,
      298, 142,
      304, 140,
      310, 139,
      313, 138,
      317, 137,
      320, 136,
      321, 136
    },
    
    // stroke 3
    {
      343, 141,
      345, 142,
      347, 142,
      349, 142,
      351, 141,
      353, 140,
      355, 138,
      356, 137,
      357, 136,
      357, 134,
      357, 133,
      357, 132,
      356, 131,
      355, 131,
      353, 131,
      351, 131,
      349, 131,
      347, 132,
      345, 133,
      344, 135,
      342, 136,
      341, 137,
      341, 139,
      340, 140,
      340, 142,
      340, 144,
      341, 145,
      342, 147,
      344, 148,
      346, 150,
      349, 151,
      352, 151,
      355, 151
    },
    
    // stroke 4
    {
      389, 106,
      388, 109,
      388, 112,
      388, 115,
      388, 119,
      388, 123,
      388, 127,
      388, 131,
      388, 135,
      388, 138,
      389, 140,
      389, 143,
      390, 145,
      390, 146      
    },
    
    // stroke 5
    {
      409, 105,
      409, 107,
      409, 109,
      409, 112,
      409, 115,
      409, 119,
      409, 124,
      411, 128,
      411, 133,
      412, 137,
      412, 140,
      413, 143,
      413, 145,
      414, 146,
    },
    
    // stroke 6
    {
      438, 129,
      438, 130,
      438, 132,
      438, 134,
      438, 136,
      438, 139,
      439, 141,
      441, 144,
      442, 146,
      444, 147,
      446, 147,
      449, 147,
      451, 146,
      454, 145,
      455, 142,
      456, 140,
      456, 137,
      455, 135,
      453, 133,
      450, 131,
      446, 131,
      443, 131,
      440, 133,
      438, 134,
      438, 136      
    },
    
    // stroke 7
    {
      504, 109,
      505, 109,
      508, 109,
      511, 109,
      515, 109,
      519, 109,
      524, 109,
      529, 108,
      534, 108      
    },
    
    // stroke 8
    {
      528, 110,
      530, 110,
      530, 111,
      530, 112,
      530, 114,
      530, 116,
      531, 118,
      531, 122,
      532, 125,
      533, 128,
      534, 132,
      535, 136,
      536, 139,
      536, 142,
      536, 146,
      535, 148,
      533, 151,
      531, 153,
      529, 154,
      526, 155,
      523, 155,
      521, 156,
      520, 156,
      518, 155,
      517, 155,
      517, 153,
      518, 152
    },
    
    // stroke 9
    {
      548, 138,
      550, 139,
      551, 140,
      551, 142,
      551, 144,
      551, 146,
      553, 148,
      554, 151,
      556, 153,
      558, 154,
      561, 154,
      564, 154,
      567, 154,
      569, 152,
      571, 150,
      573, 147,
      573, 144,
      573, 141,
      571, 139,
      569, 137,
      566, 135,
      563, 134,
      559, 134,
      556, 134,
      553, 135,
      551, 137,
      550, 138,
      550, 140
    },
    
    // stroke 10
    {
      584, 138,
      587, 139,
      589, 139,
      590, 139,
      593, 139,
      595, 139,
      598, 138,
      601, 137,
      603, 136,
      605, 134,
      607, 132,
      608, 130,
      608, 129,
      608, 127,
      607, 126,
      606, 125,
      604, 125,
      602, 125,
      599, 125,
      597, 125,
      595, 127,
      593, 129,
      591, 131,
      589, 133,
      588, 135,
      587, 138,
      587, 140,
      587, 143,
      587, 145,
      587, 148,
      587, 150,
      587, 153,
      588, 155,
      589, 157,
      591, 159,
      604, 160
    },
    
    // stroke 11
    {
      660, 104,
      660, 103,
      660, 102,
      660, 101,
      660, 102,
      660, 103,
      660, 106,
      660, 109,
      660, 113,
      660, 117,
      660, 123,
      660, 128,
      660, 133,
      661, 138,
      661, 143,
      661, 146,
      661, 149,
      661, 151,
      661, 152,
      661, 153,
      661, 151,
      661, 149,
      661, 147,
      662, 145,
      663, 142,
      665, 139,
      666, 137,
      668, 134,
      670, 132,
      672, 131,
      674, 131,
      677, 131,
      680, 132,
      683, 134,
      685, 137,
      688, 140,
      691, 143,
      692, 147,
      694, 150,
      696, 153,
      696, 155,
      697, 157,
      697, 158,
      698, 158,
      698, 157,
      698, 155
    },

    // stroke 12
    {
      715, 134,
      715, 136,
      715, 138,
      715, 140,
      715, 142,
      715, 144,
      716, 146,
      717, 148,
      719, 149,
      721, 150,
      723, 150,
      726, 150,
      728, 149,
      730, 148,
      732, 146,
      732, 143,
      732, 141,
      732, 139,
      730, 137,
      728, 135,
      726, 134,
      724, 134,
      721, 134,
      719, 134,
      717, 135,
      714, 136,
      713, 138,
      711, 140,
      710, 142,
      710, 143,
      710, 145,
      710, 147
    },

    // stroke 13
    {
      739, 133,
      741, 133,
      741, 135,
      743, 137,
      743, 140,
      745, 143,
      746, 146,
      747, 149,
      749, 152,
      751, 154,
      754, 155,
      756, 155,
      758, 155,
      760, 153,
      762, 151,
      763, 145,
      765, 143,
      765, 141,
      765, 140,
      766, 140,
      766, 141,
      767, 142,
      769, 144,
      771, 145,
      773, 146,
      775, 146,
      776, 146,
      778, 146,
      779, 144,
      779, 142,
      779, 140,
      779, 137,
      777, 134,
      775, 131,
      773, 128,
      771, 125,
      768, 122,
      766, 120,
      765, 119,
      765, 118,
      764, 118
    },
    
    // stroke 14
    {
      827, 123,
      826, 122,
      825, 122,
      824, 121,
      822, 121,
      820, 121,
      818, 121,
      816, 123,
      814, 124,
      812, 126,
      810, 128,
      808, 131,
      807, 134,
      806, 137,
      805, 140,
      805, 143,
      805, 146,
      805, 148,
      807, 151,
      809, 152,
      811, 153,
      814, 153,
      817, 151,
      821, 149,
      824, 147,
      827, 144,
      829, 141,
      831, 137,
      833, 135,
      833, 132,
      833, 130,
      833, 128,
      833, 127,
      833, 126,
      831, 125,
      831, 126,
      831, 128,
      832, 129,
      833, 131,
      835, 133,
      838, 135,
      841, 137,
      843, 140,
      846, 141,
      849, 142,
      852, 142,
      855, 142,
      857, 141
    },
    
    // stroke 15
    {
      858, 124,
      857, 122,
      857, 123,
      858, 124,
      859, 126,
      860, 129,
      862, 131,
      863, 134,
      865, 136,
      867, 138,
      868, 139,
      868, 140,
      869, 140,
      870, 140,
      870, 139,
      870, 138,
      870, 137,
      869, 136,
      869, 134,
      869, 133,
      869, 131,
      867, 129,
      867, 128,
      866, 126,
      866, 125,
      864, 124,
      864, 122,
      864, 121,
      864, 120,
      867, 120,
      869, 119,
      871, 119,
      875, 118,
      878, 118,
      882, 117,
      885, 116,
      888, 116
    },
    
    // stroke 16
    {
      914, 122,
      914, 123,
      914, 124,
      914, 125,
      915, 126,
      917, 128,
      919, 128,
      921, 128,
      923, 128,
      925, 126,
      927, 125,
      929, 124,
      931, 122,
      931, 120,
      931, 119,
      931, 118,
      929, 117,
      927, 117,
      925, 117,
      922, 117,
      920, 118,
      917, 119,
      915, 121,
      913, 123,
      912, 125,
      912, 128,
      911, 130,
      911, 132,
      912, 134,
      914, 136,
      915, 138,
      918, 139,
      920, 140,
      923, 140,
      927, 140
    },
    
    // stroke 17
    {
      988, 122,
      986, 122,
      986, 123,
      986, 124,
      986, 125,
      986, 126,
      986, 128,
      987, 130,
      988, 131,
      990, 132,
      993, 132,
      995, 132,
      997, 130,
      999, 128,
      1001, 127,
      1002, 125,
      1003, 123,
      1003, 121,
      1003, 120,
      1003, 122,
      1004, 124,
      1006, 126,
      1008, 130,
      1010, 134,
      1013, 138,
      1015, 143,
      1018, 147,
      1019, 152,
      1019, 156,
      1019, 160,
      1017, 163,
      1014, 165,
      1010, 167,
      1005, 169,
      1000, 170,
      996, 170,
      992, 170,
      990, 169,
      988, 167
    },
    
    // stroke 18
    {
      1018, 133,
      1019, 133,
      1020, 135,
      1020, 136,
      1022, 138,
      1023, 139,
      1025, 141,
      1026, 141,
      1028, 141,
      1030, 141,
      1032, 141,
      1034, 140,
      1035, 139,
      1036, 137,
      1036, 135,
      1036, 133,
      1036, 132,
      1034, 130,
      1032, 129,
      1030, 128,
      1028, 128,
      1026, 128,
      1024, 128,
      1022, 128,
      1020, 130,
      1019, 131,
      1019, 132,
      1019, 133
    },
    
    // stroke 19
    {
      1038, 124,
      1039, 125,
      1039, 126,
      1041, 127,
      1042, 129,
      1043, 131,
      1045, 133,
      1047, 135,
      1049, 136,
      1051, 137,
      1053, 137,
      1055, 137,
      1057, 136,
      1058, 134,
      1059, 132,
      1059, 130,
      1060, 128,
      1060, 126,
      1060, 124,
      1060, 123,
      1059, 123,
      1059, 124,
      1059, 126,
      1060, 128,
      1062, 130,
      1063, 132,
      1065, 134,
      1067, 135,
      1069, 137,
      1072, 137
    },
    
    // stroke 20
    {
      1088, 95,
      1088, 94,
      1088, 93,
      1090, 92,
      1092, 92,
      1095, 92,
      1098, 92,
      1101, 92,
      1104, 92,
      1106, 93,
      1109, 95,
      1111, 98,
      1113, 101,
      1113, 104,
      1114, 108,
      1114, 112,
      1112, 115,
      1111, 118,
      1109, 120,
      1108, 122,
      1106, 124,
      1105, 126,
      1103, 128,
      1102, 129,
      1102, 130,
      1102, 131,
      1104, 131
    },
    
    // stroke 21
    {
      1120, 149,
      1121, 149,
      1122, 150,
      1122, 151,
      1122, 152,
      1121, 153,
      1120, 154,
      1120, 155,
      1120, 154,
      1120, 153
    },
    
    // stroke 22
    {
      495, 136,
      496, 136,
      496, 137,
      498, 137,
      499, 137,
      500, 137,
      501, 137,
      502, 137,
      504, 137,
      505, 137,
      507, 137,
      509, 137,
      511, 137,
      513, 136,
      515, 136,
      516, 135,
      518, 135,
      520, 135,
      521, 134,
      522, 134,
      524, 134,
      525, 134,
      527, 134,
      528, 134,
      529, 134,
      531, 134,
      533, 134,
      535, 134,
      536, 134,
      538, 134,
      540, 134,
      542, 134,
      543, 133,
      545, 133,
      546, 133,
      548, 133,
      549, 133,
      551, 133,
      553, 133,
      555, 133,
      556, 133,
      558, 133,
      560, 133,
      562, 133,
      563, 133,
      565, 133,
      567, 134,
      568, 134,
      570, 134,
      571, 134,
      573, 134,
      574, 134,
      575, 134,
      577, 134,
      579, 134,
      580, 134,
      582, 134,
      583, 134,
      585, 134,
      586, 134,
      588, 135,
      589, 135,
      590, 135,
      592, 135,
      593, 135,
      595, 135,
      596, 135,
      597, 135,
      599, 136,
      600, 136,
      602, 136,
      604, 136,
      605, 136,
      607, 136,
      608, 136,
      610, 136,
      611, 136,
      612, 136,
      614, 136,
      614, 137,
      615, 137,
      616, 137,
      617, 137,
      618, 137,
      619, 137,
      621, 137,
      622, 137,
      623, 137,
      625, 138,
      626, 138,
      627, 138,
      628, 138,
      627, 138
    },
    
    // stroke 23
    {
      481, 65,
      482, 65,
      483, 65,
      486, 65,
      488, 65,
      492, 65,
      497, 64,
      501, 64,
      506, 64,
      511, 64,
      516, 62,
      519, 62,
      522, 62      
    },
    
    // stroke 24
    {
      508, 63,
      507, 63,
      507, 64,
      506, 65,
      506, 66,
      506, 67,
      506, 69,
      506, 71,
      506, 73,
      506, 75,
      507, 78,
      508, 81,
      510, 84,
      511, 86,
      512, 89,
      512, 91,
      512, 93,
      512, 95,
      510, 97,
      508, 99,
      506, 99,
      503, 100,
      500, 100,
      498, 101,
      496, 101,
      494, 101,
      494, 100
    },
    
    // stroke 25
    {
      526, 86,
      526, 89,
      526, 90,
      527, 91,
      528, 92,
      530, 93,
      531, 93,
      533, 94,
      535, 94,
      537, 93,
      539, 92,
      541, 91,
      542, 90,
      542, 89,
      542, 88,
      540, 86,
      539, 86,
      536, 86,
      534, 86,
      531, 86,
      528, 86,
      526, 86,
      524, 87,
      523, 88,
      523, 90,
    },
    
    // stroke 26
    {
      546, 68,
      546, 69,
      547, 71,
      547, 73,
      548, 75,
      549, 78,
      550, 81,
      551, 83,
      552, 86,
      554, 88,
      555, 89,
      556, 90,
      556, 91,
      557, 91,
      557, 90,
      557, 89,
      557, 88,
      558, 87,
      559, 86,
      560, 84,
      562, 83,
      563, 83,
      565, 82,
      567, 82,
      568, 82,
      570, 82,
      572, 82,
      573, 83,
      574, 84,
      576, 86,
      577, 87,
      577, 89,
      578, 90,
      578, 91,
      579, 92
    },
    
    // stroke 27
    {
      587, 80,
      588, 81,
      590, 82,
      590, 83,
      591, 84,
      591, 85,
      592, 85,
      592, 86,
      592, 85,
      592, 84,
      591, 83,
      591, 82,
      591, 81,
      591, 80,
      592, 80,
      592, 79,
      594, 79,
      596, 78,
      597, 78,
      599, 78,
      601, 79,
      603, 80,
      604, 82,
      606, 84,
      608, 86,
      610, 88,
      611, 90,
      612, 92,
      613, 93,
      613, 95,
      614, 95,
      614, 96      
    }
  };  
} // DigitalInkTaggingWithRecognition
