// helper class to translate text
// Yves Schutz March 2019

#include <iostream>

#include "MCMultiLingual.h"
#include <TGMsgBox.h>
#include <TObjString.h>

MCMultiLingual MCMultiLingual::fInstance = MCMultiLingual();

//=MCMultiLingual========================================================================
MCMultiLingual::MCMultiLingual()
{
   fLanguage = L_FR;

   fFrancais.push_back("Demo");
   fEnglish.push_back("Demo");
   fFrancais.push_back("Etudiant ");
   fEnglish.push_back("Student");
   fFrancais.push_back("Tuteur");
   fEnglish.push_back("Tutor");
   fFrancais.push_back("Données");
   fEnglish.push_back("Data");
   fFrancais.push_back("Lot");
   fEnglish.push_back("Set");
   fFrancais.push_back("Quoi faire ?");
   fEnglish.push_back("Instructions");
   fFrancais.push_back("\
Bienvenu dans l'exercice Masterclass de ALICE !\n\n\
Dans la fenetre <MasterClass>, il y a 3 boutons a votre disposition\n\
En cliquant ces boutons, vous pouvez choisir l'un des modes suivant :\n\n\
  Demo     - demarre une demonstration de l'exercice MasterClass analysant 4 evenements\n\
\
  Etudiant - demarre l'exercice MasterClass avec 100 evenemets reels que vous devrez analyser\n\
\
  Tuteur   - demarre l'exercice Masterclass avec des outils permettant de finaliser \n\
             l'exercice a partir des resultats obtenus par les etudiants et de conclure");
   fEnglish.push_back("\
Welcome to ALICE Masterclass!\n\n\
\
You see the window <MasterClass>.There are 3 buttons to your disposal\n\
Using these buttons, one can choose the mode:\n\n\
  Demo    - starts a demo of MasterClass exercise with 4 events to analyse\n\
\
  Student - starts MasterClass with 100 real events for you to analyze\n\
\
  Tutor   - starts Masterclass with Tutor tools that allow to summarise\n\
            the students results and draw conclusions from the analysis");
   fFrancais.push_back("Veuillez sélectionner un lot de données");
   fEnglish.push_back("Select a data set");
   fFrancais.push_back("INSTRUCTIONS MODE ETUDIANT");
   fEnglish.push_back("STUDENT MODE INSTRUCTIONS");
   fFrancais.push_back("Fermer");
   fEnglish.push_back("Close");
   fFrancais.push_back("Effacer");
   fEnglish.push_back("Clear");
   fFrancais.push_back("Evenement analyse !");
   fEnglish.push_back("Event analysed!");
   fFrancais.push_back("Evenements deja analyses :");
   fEnglish.push_back("Events already analyzed:");
   fFrancais.push_back("Visu");
   fEnglish.push_back("Display");
   fFrancais.push_back("Grappes");
   fEnglish.push_back("Clusters");
   fFrancais.push_back("Traces");
   fEnglish.push_back("Tracks");
   fFrancais.push_back("Geometrie");
   fEnglish.push_back("Geometry");
   fFrancais.push_back("Fond d'ecran");
   fEnglish.push_back("Screen Background");
   fFrancais.push_back("Encyclopedie");
   fEnglish.push_back("Encyclopaedia");
   fFrancais.push_back("Detecteur ALICE");
   fEnglish.push_back("ALICE Detector");
   fFrancais.push_back("Centralite ");
   fEnglish.push_back("Centrality");
   fFrancais.push_back("Quantite de mouvement");
   fEnglish.push_back("Momentum");
   fFrancais.push_back("Nombre de coups");
   fEnglish.push_back("Counts");
   fFrancais.push_back("Evenements");
   fEnglish.push_back("Events");
   fFrancais.push_back("Choisir");
   fEnglish.push_back("Select");

   // MCMAIN
   fFrancais.push_back("Voulez-vous vraiment quitter ?");
   fEnglish.push_back("Do you want really to quit?");
   //MCMultiView
   fFrancais.push_back("Vue Multiple");
   fEnglish.push_back("Multi View");
   fFrancais.push_back("Vue 3D");
   fEnglish.push_back("3D View");
   fFrancais.push_back("Vue Rho-Z");
   fEnglish.push_back("Rho-Z View");
   fFrancais.push_back("Vue R-Phi");
   fEnglish.push_back("R-Phi View");

   // MCExeciseS1
   fFrancais.push_back("Masse Invariante (GeV/c^{2})");
   fEnglish.push_back("Invariant Mass (GeV/c^{2})");
   fFrancais.push_back("Masse Invariante - resultat final");
   fEnglish.push_back("Invariant Mass - final result");

   // MCExeciseS1
   fFrancais.push_back("utiliser la molette  pour zoomer");
   fEnglish.push_back("use ROLLER to zoom in/out");
   fFrancais.push_back("click gauche pour tourner");
   fEnglish.push_back("click LEFT button to rotate");
   fFrancais.push_back("maintenir la molette pour déplacer");
   fEnglish.push_back("press ROLLER to move");
   fFrancais.push_back("Master Class : Etrangete Exercice 1");
   fEnglish.push_back("Master Class: Strangeness Exercice 1");
   fFrancais.push_back("Bonjour, ceci est la partie 1 de l'exercice strangeness");
   fEnglish.push_back("Hello, this is strangeness exercice part1");
   fFrancais.push_back("Rapidite - resultat final");
   fEnglish.push_back("Rapidity - final result");
   fFrancais.push_back("Rapidite");
   fEnglish.push_back("Rapidity");
   fFrancais.push_back("Voulez-vous vraiment quitter ?");
   fEnglish.push_back("Do you want really to quit?");
   fFrancais.push_back("INSTRUCTIONS MODE DEMO");
   fEnglish.push_back("DEMO MODE INSTRUCTIONS");
   fFrancais.push_back("INSTRUCTIONS MODE TUTEUR");
   fEnglish.push_back("TUTOR MODE INSTRUCTIONS");
   fFrancais.push_back("Instructions Analyse");
   fEnglish.push_back("Analysis Instructions");
   fFrancais.push_back("\
Bienvenu.e dans le mode <Demo>, vous y trouverez les outils necessaires a l'analyse des\n\
evenements ALICE. Dans ce mode il n'y aura que 4 evenements pour l'analyse et chacun\n\
d'eux contient une particule etrange. Le nom de la particule est affiche dans le\n\
bas de la vue 3D se trouvant a droite\n\n\
\
Sur la droite sont affiches les evenements vus de trois facons differentes.\n\
Sur la gauche se trouve votre poste de pilotage divise en plusieurs parties :\n\
\
Navigation Evenements :\n\
   Cliquez les fleches pour selectioner l'evenement precedent ou suivant.\n\
   Le numero de l'evenement courant et le nombre d'evenements analyses sont egalement affiches.\n\n\
\
Particules Etranges - ceci vous permet d'afficher ou non les traces issues de la decroissance \n\
de particules etranges :\n\
   V0s      - traces produites a partir d'une decroissance V0 qu'il vous est demande de rechercher\n\
   Cascades - traces d'une decroissance cascade qu'il vous est demande de rechercher\n\
   (NOTE: L'affichage de l'objet est confirme par la marque dans la case a cocher\n\
   a droite)\n\n\
\
Calculateur:\n\
   Calculateur           - vous permet de calculer les proprietes des particules etranges telles que\n\
   la masse invariante a partir des caracteristiques des particules filles\n\
   Tableau des Resultats - tableau resumant la statistique des particules etranges \n\
\n\
\
Visualisation - vous permet de visualiser ou non des elements tels que :\n\
   Vertex     - le point ou s'est produit la collision\n\
   Grappes    - les points de passage des particules dans les detecteurs\n\
   Traces     - la trajectoire reconstruite des particules traversant les detecteurs\n\
   Geometrie  - pour afficher les detecteurs ALICE\n\
   Fond       - pour choisir la couleur d'arriere plan du panneau de visualisation\n\
   Axes       - systeme de reference de l'experience ALICE\n\
   (NOTE: L'affichage de l'objet est confirme par la marque dans la case a cocher\n\
   a droite)\n\n\
\
Encyclopedie :\n\
   Detecteur ALICE              - description de ALICE et de ses principaux composants;\n\
   Topologie de decroissance V0 - topologie de decroissance des particules a rechercher;\n\
   (NOTE: cliquer les images des detecteurs/decroissances pour obtenir \n\
	d'avantage d'informations)");
   fEnglish.push_back("\
Welcome to the <Demo> mode, here you see all the tools you need to analyse\n\
the ALICE Events. In this mode, there are only 4 events to analyze, and each of\n\
them contains a strange particle. The name of the particle is displayed in the\n\
bottom of the 3D View on the right-hand side\n\n\
\
On the right hand side you can see the events in 3 views.\n\
On the left hand side you have your steering board which is divided into segments:\n\
\
Instructions:\n\
   Instructions          - general instructions of how to use the application\n\
   Analysis Instructions - instructions for analysis\n\
\n\n\
\
Event Navigation:\n\
   Click on the arrows to go to the previous or to the next event.\n\
   The current event number and number of analysed events are also displayed.\n\n\
\
Strange Particles - this allows you to display or hide tracks from decays\n\
of strange particles:\n\
   V0s      - tracks produced from a V0 decays you will be looking for\n\
   Cascades - tracks from the cascade decays you will be looking for\n\
   (NOTE: The appearance of the object is confirmed by the tick in the checkbox\n\
   on the right)\n\n\
\
Calculator:\n\
   Calculator       - allows you to calculate the properties of strange particles like their\n\
   invariant mass given the daughter particles propreties\n\
   Table of Results - table with strange particle statistics \n\
   \n\
\
Display - this allows you to display or hide elements like:\n\
   Vertex     - the point where the collision took place\n\
   Clusters   - reconstructed points in the detectors showing where the particle was\n\
   Tracks     - reconstructed trajectories of the particles traversing the detectors\n\
   Geometry   - drawing of ALICE detectors\n\
   Axes       - reference system of ALICE Experiment\n\
   Background - choose the colour of the display panel\n\
   (NOTE: The appearance of the object is confirmed by the tick in the checkbox\n\
   on the right)\n\n\
\
Encyclopedia:\n\
   ALICE detector    - description of ALICE and its main components;\n\
   V0 Decay Patterns - patterns of the particles you will be looking for;\n\
   (NOTE: click on the pictures of the detectors/decays to get further\n\
   information)");
   fFrancais.push_back("\
Bienvenu.e dans le mode <Etudiant>, vous y trouverez les outils necessaires a l'analyse des\n\
evenements ALICE.\n\n\
\
Sur la droite sont affiches les evenements vus de trois facons differentes.\n\
Sur la gauche se trouve votre poste de pilotage divise en plusieurs parties :\n\
\
Instructions :\n\
   Instructions Globales - instructions generales pour l'utilisation de l'application\n\
   Instructions Analyse  - instructions pour l'analyse\n\
 \n\n\
\
Navigation Evenements :\n\
   Cliquez les fleches pour selectioner l'evenement precedent ou suivant.\n\
   Le numero de l'evenement courant et le nombre d'evenements analyses sont egalement affiches.\n\n\
\
Particules Etranges - ceci vous permet d'afficher ou non les traces issues de la decroissance \n\
de particules etranges :\n\
   V0s      - traces produites a partir d'une decroissance V0 qu'il vous est demande de rechercher\n\
   Cascades - traces d'une decroissance cascade qu'il vous est demande de rechercher\n\
   (NOTE: L'affichage de l'objet est confirme par la marque dans la case a cocher\n\
   a droite)\n\n\
\
Calculateur:\n\
   Calculateur - vous permet de calculer les proprietes des particules etranges telles que\n\
   la masse invariante a partir des caracteristiques des particules filles\n\
   Tableau des Resultats - tableau resumant la statistique des particules etranges \n\
  \n\
\
Visualisation - vous permet de visualiser ou non des elements tels que :\n\
   Vertex     - le point ou s'est produit la collision\n\
   Grappes    - les points de passage des particules dans les detecteurs\n\
   Traces     - la trajectoire reconstruite des particules traversant les detecteurs\n\
   Geometrie  - pour afficher les detecteurs ALICE\n\
   Fond       - pour choisir la couleur d'arriere plan du panneau de visualisation\n\
   Axes       - systeme de reference de l'experience ALICE\n\
   (NOTE: L'affichage de l'objet est confirme par la marque dans la case a cocher\n\
   a droite)\n\n\
\
 Encyclopedie :\n\
	Detecteur ALICE - description de ALICE et de ses principaux composants;\n\
	Topologie de decroissance V0 - topologie de decroissance des particules a rechercher;\n\
	(NOTE: cliquer les images des detecteurs/decroissances pour obtenir \n\
	d'avantage d'informations)");
   fEnglish.push_back("\
the ALICE Events.\n\n\
\
On the right hand side you can see the events in 3 views.\n\
On the left hand side there is your steering board which is divided into segments:\n\
\
Instructions:\n\
   Global Instructions   - general instructions of how to use the application\n\
   Analysis Instructions - instructions for analysis\n\
 \n\n\
\
Event Navigation:\n\
   Click on the arrows to go to the previous or to the next event.\n\
   The current event number and number of analysed events are also displayed.\n\n\
\
Strange Particles - this allows you to display or hide tracks from decays\n\
of strange particles:\n\
   V0s      - tracks produced from a V0 decays you will be looking for\n\
   Cascades - tracks from the cascade decays you will be looking for\n\
   (NOTE: The appearance of the object is confirmed by the tick in the checkbox\n\
   on the right)\n\n\
\
Calculator:\n\
   Calculator       - allows you to calculate the properties of strange particles like\n\
   invariant mass given the daughter particles characteristics\n\
   Table of Results - table with strange particle statistics \n\
  \n\
\
Display - this allows you to display or hide elements like:\n\
   Vertex     - the point where the collision took place\n\
   Clusters   - reconstucted points in the detectors showing where the particle was\n\
   Tracks     - reconstructed trajectories of the particles traversing the detectors\n\
   Geometry   - drawing of ALICE detectors\n\
   Background - choose the colour of the display panel\n\
   Axes       - reference system of ALICE Experiment\n\
   (NOTE: The appearance of the object is confirmed by the tick in the checkbox\n\
   on the right)\n\n\
\
Encyclopedia:\n\
   ALICE detector - description of ALICE and its main components;\n\
   V0 Decay Patterns - patterns of the particles you will be looking for;\n\
   (NOTE: click on the pictures of the detectors/decays to get further\n\
   information)");
   fFrancais.push_back("\
Bienvenu.e dans le mode <Tuteur>, vous y trouverez les outils pour fusionner les résultats et faire l'analyse\n\
de vos résultats obtenus en mode <Etudiant>\n\n\
\
Dans le panneau de gauche, vous voyez quatre autres panneaux appelés <Instructions Tuteurs>,\n\
<Controles Tuteur>, <Resultats> and <Encyclopedie> :\n\n\
\
   Instructions - decrit comment utiliser l'application en mode <Tuteur> mode\n\
   Controles Tuteur - vous permet de fusionner et analyser les resultats obtenus en mode <Etudiant>.\n\
      Obtenir les Fichiers - vous permet de recuperer les fichiers des resultats obtenus en mode <Etudiant>\n\
      Enregistrer les Histogrammes - enregistre les histogrammes montrant les resultats obtenus en mode <Etudiant>\n\
   Resultats\n\
      Tableau des Resultats - affiche et interprete les resultats de l'analyse\n\
    \n\
Encyclopedie :\n\
   Detecteur ALICE - description de ALICE et de ses principaux composants;\n\
   Topologie de decroissance V0 - topologie de decroissance des particules a rechercher;\n\
   (NOTE: cliquer les images des detecteurs/decroissances pour obtenir \n\
   d'avantage d'informations)\n\n\
\
Animation de l'Evenement :\n\
   Play/Stop - Observer la propagation des particules traversant les detecteurs\n\n\
\
\
\n\n\
\
Comment fusionner et analyser les resultats obtenus en mode <Etudiant>\n\n\
\
   1. En cliquant le bouton <Obtenir les Fichiers> une fenetre <Ouvrir> s'affiche.\n\
   2. Trouvez le resultat (type de fichier *.masterclass) et cliquez <Ouvrir>. Le resultat\n\
      will be loaded. Repeat it for all the students' files\n\
   3. Cliquez l'onglet  <Masse Invariante Fusionnee> (à droite) pour afficher\n\
      la masse invariante des particules etranges decouvertes par votre etudiant.\n\
      Ajoutez à l'histogramme en cliquant chacune d'elles.\n\
   4. Cliquez <Enregistrez les Histogrammes>, donnez un nom à l'image et <Save>\n\
   5. Cliquez <Tableau des Resultats>. Un tableau appele <Statistique des Particules Etrange> s'affichera\n\
   6. Comparez les resultats calcules avec des simulations Monte-Carlo avec les \n\
      resultats obtenus en mode <Eudiant>. Est-ce-que les resultats concordent ? Oui ? Non ? Que cela signifie-t-il ?\n\
   7. Fermer <Statistiques des Particules Etranges> en cliquant <Fermer>\n\
 \n\
   8. Cliquez <Fermer>. L'analyse est finie !\n\n\
\
    ");
   fEnglish.push_back("\
Welcome in the <Teacher> mode, here you can see the tools to merge and analyse\n\
your students' results\n\n\
\
On the left hand side panel you can see four panels, called <Teacher Instructions>,\n\
<Teacher Controls>, <Results> and <Encyclopaedia>:\n\n\
\
   Instructions - desribes how to operate the application in the <Teacher> mode\n\
   Teacher controls - allows you to merge and analayze students' results.\n\
      Get Files - this allows to get your students' results\n\
      Save Histograms - saves histograms showing your students' results\n\
   Results\n\
      Table of Results - shows and interprets the analysis results\n\
    \n\
Encyclopedia:\n\
   ALICE detector - description of ALICE and its main components;\n\
   V0 Decay Patterns - patterns of the particles you will be looking for;\n\
   (NOTE: click on the pictures of the detectors/decays to get further\n\
   information)\n\n\
\
Event animation:\n\
   Play/Stop - Watch the charged particles propagation through the detector\n\n\
\
\
In the bottom, there are main application control buttons:\n\
   Student Mode - starts the application in the Student mode,\n\
   Exit\n\n\
\
How to merge and analyze the students' results\n\n\
\
   1. Click the button <Get Files> and a window <Open> will appear.\n\
   2. Find the result (filetype *.masterclass) and click <Open>. The result\n\
      will be loaded. Repeat it for all the students' files\n\
   3. Click on the tab <Merged Invariant Mass> (right-hand side) to see\n\
      the invariant mass of the strange particles found by your students.\n\
      Update the histograms by clicking on each of them.\n\
   4. Click <Save Histograms>, give the title of the pictures and <Save>\n\
   5. Click <Table of Results>. A table called <Strange Particle Statistics> appears\n\
   6. Compare the pre-calculated results from Monte Carlo simulations with the\n\
      results of your students. Do the results agree? Yes? No? What Does it mean?\n\
   7. Close <Strange Particle Statistics> by clicking <Close>\n\
 \n\
   8. Click <Close>. The analysis is done!\n\n\
\
    ");
   fFrancais.push_back("\
But de l'analyse :\n\n\
\
La production de particules etranges est rare et elles ne sont pas produites dans chaque evenement\n\
Vous avez 100 evenemets ; examinez chacun d'eux et comptez\n\
combien de particules etranges de chaque espece vous pouvez trouver (combien de Kaons,\n\
Lambdas, antiLambdas, Xis) et comparez vos resultats aux donnes MC.\n\n\
\
Comment analyser un evenement :\n\n\
\
   Cliquez <Calculateur>\n\n\
\
   1. Utilisez les boutons suivants pour afficher les objets des donnees ALICE.\n\
\
     1.1 cliquez <V0> et <Cascades> dans la rubique <Pricules Etranges>\n\
     1.2 (Option) Cliquez <Vertex>, >Clusters> et <Traces> dans la rubique <Visualisation>\n\
     les traces des particules de decroissance des particules sont marquees en rouge, vert et bleu,\n\
     et les traves des autres particules (principalement des pions) sont marquees en gris.\n\n\
\
   2. Regardez attentivement la visualisation de l'evenement. Reconnaissez-vous un motif interessant\n\
      qui ressemble a une decroissance V0 ou cascade ?\n\
      Si vous avez des difficultes pour identifier les traves des particules etranges, supprimer l'affichage des autres particules.\n\
     2.1 Si vous avez trouve une topologie V0, cliquant chacune des deux traces\n\
         resultant de la decroissance. Vous obtientrez alors une fenetre avec la quantite de mouvement,\n\
         la masse et la charge de cette particule.\n\
	 2.2 Si vous avez trouve une topologie cascade, cliquez chacune des trois traces\n\
         resultant de la decroissance. Vous obtientrez alors une fenetre avec la quantite de mouvement,\n\
         la masse et la charge de cette particule.\n\
\
   2. Cliquez <Copy to Calculator>. la quantite de mouvement et la masse de chaque  particule seront copiees\n\
      dans le Calculateur.\n\n\
\
   Maintenant, verifiez la masse des partcules. Quelles sont-elles ?\n\
   Vous le rappelez vous du cours ? Si non, vous les trouverez en haut\n\
   du panneau du Calculateur dans <Tableau des Particules>.\n\n\
\
   Cliquez ensuite <Masse Invariante>. La masse invariante\n\
   de la particule mere sera calculee. Est-ce que la masse invariante correspon à l'une\n\
   des masses dans le tableau des particules ? Si oui, vous avez trouve une particule etrange ! (Si non, c'est du bruit de fond).\n\n\
\
   Confirmez en cliquant <Ceci est un XXX>, ou XXX est le nom de la particule que vous avez trouvee.\n\n\
\
   Lorsque vous avez trouve une particule, cliquez l'onglet <Masse Invariante> au-dessus de la visualition de l'evenemet.\n\
   Pour mettre a jour vos resultats dans les histogrammes\n\n\
\
   4. Lorsque vous avez termine l'analyse de l'evenement, cliquez <Evenement analyse !> pour compter les evenements analyses.\n\
   Continuez ensuite avec l'evenement suivant en cliquant la fleche vers la droite dans la rubrique <Vavigation Evenement>.\n\n\
\
   5. Lorsque vous aurez analyse tous les evenements, cliquez <Tableau des Resultats>. Il y a deux series de statistique\n\
      de particules etranges, l'une est celle predite par les modeles theoriques et l'autre\n\
	  est le resultat de votre analyse. Est-ce que les deux sont en accord ? Si oui, la theorie decrit bien la realite et\n\
	  les mecanismes de la 'physique standart' decrite dans le MC sont valides.\n\n\
\
	  ATTENTION :\n\
      Les particules etranges sont rares. Si nous vous avions donne les evenements que nous analysons d'habitude (reels + MC),\n\
      vous auriez du examiner 100 evenements pour trouver un Lambda et 1000 pour trouver un Xi. Pour vous simplifier la tache\n\
      nous avons elimine des evenements ne contenant aucune particule etrange.\n\
      Pour obtenir les nombres finals vous devez multiplier vos resultats par les facteurs suivants :\n\
      (a) Kaon x 100\n\
      (b) Lambda x 100\n\
      (c) antiLambda x 100\n\
      (d) Xi x 1000\n\n\
\
   6. Cliquez <Sauvegarder> dans l'outil Calculateur.\n\n\
\
   7. Transmettez le fichier avec vos resultats a votre tuteur : Copier le fichier avec vos resultats\n\
      sur la cle memoire\n\n\
\
      ");
   fEnglish.push_back("\
Analysis aim:\n\n\
\
Strange particles are rare and they are not produced in every event\n\
You are given 100 events; go through each one carefully and count\n\
how many strange particles of each species you can find (how many Kaons,\n\
Lambdas, antiLambdas, Xis) and compare them to MC data.\n\n\
\
How to analyse an event:\n\n\
\
   Click on <Calculator>\n\n\
\
   1. Use the following buttons to display objects of the ALICE data.\n\
\
   1.1 Click on <V0> and <Cascades> in <Strange Particles>\n\
   1.2 (Option) Click on <Vertex>, <Clusters> and <Tracks> in <Display>\n\
   The tracks from strange particle decays have been coloured in red, green and blue,\n\
   while tracks of other particles (mostly pions) have been coloured grey.\n\n\
\
   2. Look carefully at the event display. Do you recognize any interesting pattern\n\
      that looks like the decay of V0s or Cascades?\n\
      If it is difficult for you to spot the strange particle tracks, take away the others.\n\
   2.1 If you recognize a V0 decay pattern, click on each one of the two tracks\n\
       that were produced from the decay. Then you get a window with the momentum,\n\
       mass and charge of this particle.\n\
   2.2 If you recognize a Cascade decay pattern, click on each one of the three tracks\n\
       that were produced from the decay. Then you get a window with the momentum,\n\
       mass and charge of this particle.\n\n\
\
   3. Click <Copy to Calculator>. The momentum and mass of each particle is copied\n\
      to the Calculator.\n\n\
\
   Now, check the masses of the particles. What are they?\n\
   Do you remember it from the lecture? If not, you will find them at the top\n\
   of the Calculator panel in the <Particle Table>.\n\n\
\
   Then, click <Invariant Mass>. Then, the invariant mass\n\
   of the mother particle is calculated. Does the invariant mass corresponds to any value\n\
   in the particle table? If so, you have found a strange particle! (If not, it's background).\n\n\
\
   Confirm it by clicking <That's XXX>, where XXX is the name of the particle you have found.\n\n\
\
   When you find a particle, click on tab over the event display <Invariant mass>.\n\
   To update your results click on the historgams\n\n\
\
   4. When you finish with one event, click <Event analysed!> to count analysed events.\n\
   Then go to the next one by clicking the right arrow in <Event Navigation> section.\n\n\
\
   5. When you finish all the events, click <Table of Results>. There are two sets of strange\n\
      particle statistics there, one as predicted by theoretical models and one with your\n\
      analysis results. Do they agree? Then the theory descibes reality, and the\n\
      'standard physics' processes mechanism described in MC is verified.\n\n\
\
      REMEMBER:\n\
      Strange particles are rare. If we had given you the events we analyse (real + MC),\n\
      you should look at 100 to find one Lambda and 1000 to find one Xi. To make it easier for you\n\
      we threw away  some of the events without strange particles in the controlled way.\n\
      To get the final numbers you have to multiply your results with some factors:\n\
      (a) Kaon x 100\n\
      (b) Lambda x 100\n\
      (c) antiLambda x 100\n\
      (d) Xi x 1000\n\n\
\
   6. Click <Save> in the calculator.\n\n\
\
   7. Pass the file with the results to your teacher: Copy the file with your results\n\
      to the memory stick\n\n\
\
   ");
   fFrancais.push_back("Master Classe ALICE - MODE TUTEUR");
   fEnglish.push_back("ALICE MASTER CLASS - TUTOR MODE");
   fFrancais.push_back("Master Classe ALICE - MODE ETUDIANT");
   fEnglish.push_back("ALICE MASTER CLASS - STUDENT MODE");
   fFrancais.push_back("Masse Invariante regroupee");
   fEnglish.push_back("Merged Invariant Mass");
   fFrancais.push_back("Instructions Globales");
   fEnglish.push_back("Global Instructions");
   fFrancais.push_back("Controle Tuteur");
   fEnglish.push_back("Tutor Control");
   fFrancais.push_back("Recuperer les fichiers");
   fEnglish.push_back("Get Files");
   fFrancais.push_back("Sauvegarder les fichiers");
   fEnglish.push_back("Save Files");
   fFrancais.push_back("Resultats");
   fEnglish.push_back("Results");
   fFrancais.push_back("Tableau des Resultats");
   fEnglish.push_back("Table of Results");
   fFrancais.push_back("Statistique des Particules Etranges");
   fEnglish.push_back("Strange Particles Statistics");
   fFrancais.push_back("Particule");
   fEnglish.push_back("Particle");
   fFrancais.push_back("Donnees Monte-Carlo");
   fEnglish.push_back("Monte-Carlo Data");
   fFrancais.push_back("Donnees Reelles");
   fEnglish.push_back("Real Data");
   fFrancais.push_back("Topologie V0");
   fEnglish.push_back("V0 Pattern");
   fFrancais.push_back("Topologies");
   fEnglish.push_back("Patterns");
   fFrancais.push_back("Info Topologies");
   fEnglish.push_back("Patterns Info");
   fFrancais.push_back("Mode Etudiant");
   fEnglish.push_back("Student Mode");
   fFrancais.push_back("Sortie");
   fEnglish.push_back("Exit");
   fFrancais.push_back("Navigation Evenements");
   fEnglish.push_back("Events Navigation");
   fFrancais.push_back("Precedent");
   fEnglish.push_back("Previous");
   fFrancais.push_back("Courant");
   fEnglish.push_back("Current");
   fFrancais.push_back("Suivant");
   fEnglish.push_back("Next");
   fFrancais.push_back("Particules Etranges");
   fEnglish.push_back("Strange Particles");
   fFrancais.push_back("Je suis pret ! Demarrer l'exercise");
   fEnglish.push_back("I am ready! Start Exercise");
   fFrancais.push_back("Retour mode Demo");
   fEnglish.push_back("Back to Demo");
   fFrancais.push_back("Vertex primaire");
   fEnglish.push_back("Primary Vertex");
   fFrancais.push_back("Copier vers calculateur");
   fEnglish.push_back("Copy to calculator");
   fFrancais.push_back("Demarrer le calculateur");
   fEnglish.push_back("Start the calculator");
   fFrancais.push_back("Dernier evenement");
   fEnglish.push_back("Last event");
   fFrancais.push_back("Rien a sauvegarder");
   fEnglish.push_back("Nothing to save");

   fFrancais.push_back("Tout decocher");
   fEnglish.push_back("Untick all");
   fFrancais.push_back("Candidats de masse invariante");
   fEnglish.push_back("Invariant mass candidates");
   fFrancais.push_back("Coups par bin de ");
   fEnglish.push_back("Counts per bin of ");
   fFrancais.push_back("Ajustage");
   fEnglish.push_back("Fit");
   fFrancais.push_back("Bruit de fond");
   fEnglish.push_back("Background");
   fFrancais.push_back("Interval fond");
   fEnglish.push_back("Bckg range");
   fFrancais.push_back("Interval pic");
   fEnglish.push_back("Peak range");
   fFrancais.push_back("Ajuste pic+fond");
   fEnglish.push_back("Fit peak+bckg");
   fFrancais.push_back("Mise a jour");
   fEnglish.push_back("Refresh");

   // MCExeciseR1
   fFrancais.push_back("\
Bienvenu.e dans l'afficheur d'evenement de ALICE, vous trouverez ici tous les outils necessaires pour analyzer\n\
les evenements ALICE.\n\n\
A droite, vous voyez les evenements sous 3 vues differentes.\n\n\
A gauche, vous voyez votre poste de commandes divise en 3 parties :\n\n\
\t * Navigation Evenement: Cliquez sur les fleches pour afficher le prochain evenement ou le precedent.\n\
\t\t Le numero de l'evenement courrant et le nombre d'evenements analyzes y sont affiches.\n\n\
\t * Compteur: Le compteur permet de compter les traces chargees dans l'evenement et les ajoute aux histogrammes\n\
\t\t de plus, il affiche les proprietes des traces,les composamtes x, y, z de la quantite de mouvements \n\
\t\t ainsi que la charge et la quantite de mouvement transverse \n\
\n\
\t * Visualisation - ceci voous permet d'afficher ou d'occulter des elements tels que:\n\
\t\t - Vertex  : \t\t le point ou la collision a eu lieu\n\
\t\t - Grappes : \t\t les poinst reconstruits dans le detecteur indiquant par ou est passee la particule\n\
\t\t - Traces  : \t\t trajectoires reconstruites de la particules lorsqu'elles traversent les detecteurs\n\
\t\t - Voir traces pimaires : limitera le nombre de traces affichees a celles qui sont issues du vertex primaire\n\
\t\t - Geometrie : \t\t affichage des detecteurs ALICE\n\
\t\t - Fond : \t\t permet de choisir la couleur du fond d'ecran\n\
\t\t - Axes : \t\t systeme de reference de l'experience ALICE\n\
(NOTE: les elements selectionnes sont marques par la presence d'une croix sur la droite)\n\n\
\t * Encyclopedie: \n\n\
\t\t - Detecteur ALICE : description de ALICE et de ses composants principaux;\n\
(NOTE: cliquez sur l'image du detecteur pour obtenir d'avantage d'informations)\n\n\
Dans la partie basse, se trouvent les boutons de controle de l'application principale :\n\
\t\t\t\t\t Exit\n\n\n\
VOTRE MISSION:  \n\
Dans cet exercice du programme MasterClass vous devez evaluer la multiplicite moyenne\n\
des evenements pp (premiers 30 evenements), et des collisions Pb-Pb peripheriques (evenement 31), semi-centraux (evenement 32)  \n\
et centraux (evenement 33) et determiner le facteur de modification nucleaire integre pour ces 3 evenemets Pb-Pb.\n \n\
Avant de commencer vous devez vous familiarisez avec cet outil. \n\n\
Dans l'afficheur d'evenements vous pouvez visualiser les grappes (points d'espace reconstruits)\n\
dans les differents detecteurs. A partir de ces grappes, les trajectoires des particules, appelee trace, traversant les\n\
detecteurs sont reconstruites. A partir des traces il est possible de reconstruire le \n\
point de la collision (vertex primaire) a partir duquel emerge la plupart des traces. Cependant, vu  \n\
la taille de certains detecteurs on peut voir plusieurs points a partir desquels emergent plusieurs particules. \n\n\
Mais la collision primaire doit avoir eu lieu tres pret du centre  \n\
de notre systeme de coordonees (X~0, Y~0, Z~0), de facon a ce que tous les detecteurs voient la collision et \n\
que les traces puissent etre bien mieux reconstruites. \n\n\
Les traces emergeant d'autre vertex sont appellees traces d'empilement et ne doivent pas etre prises en compte pour le calcul \n\
de la multiplicite de l'evenement. \n\n\
C'est pour cela qu'il vous suffira de visualiser uniquement les traces primaires (issues du vertex primaire) en cliquant le bouton \n\
\t \"Voir les traces primaires\". \n\
En cliquant a nouveau, la totalite de l'evenement s'affichera a nouveau.\n\n\
Pour atteindre l'objectif final, vous devez utiliser le \"Compteur\". Cet outil vous aidera a compter le \n\
nombre de traces primaires. Il incrementera la multiplicite de l'evenemet courant d'une unite en cliquant \n\
une trace, qui changea de couleur (rouge) pour vous eviter les doubles comptages.\n\n\
Apres avoir clique le bouton \"Evenement analyze\" la multiplicte de cet evenement sera ajoutee \n\
a l'histogramme sous l'onglet \"Caracteristiques de l'evenement\". \n\n\
Sous l'onglet \"Caracteristiques de l'evenement\" les histogrammes sont positionnes comme suit : \n\n \
en haut a gauche :\t\t multiplicite par evenement ou nombre de traces\n \
en haut a droite :\t\t multiplicite par evenement, pour les traces ayant une quantite de mouvement transverse au-dessus d'un seuil (pt)\n \
\t\t\t\t superieur a 1.0 GeV/c \n \
en bas a gauche  :\t\t distribution de la quantite de mouvement transverse pour chaque trace analysee \n \
en bas a droite  :\t\t signe de la charge de chaque trace analysee\n\n\
La multiplicite moyenne deduite soit pour seuil en pt soit sans seuil est affichee\n\
dans l'encart de l'histogramme correspondant.\
				");
   fEnglish.push_back("\
Welcome to the ALICE event-diplay, here you see all the tools you need to analyse\n\
the ALICE Events.\n\n\
\
On the right hand side you can see the events in 3 views.\n\
On the left hand side there is your steering board which is divided into segments:\n\
Event Navigation:\n\
Click on the arrows to go to the previous or to the next event.\n\
The current event number and number of analysed events are also displayed.\n\n\
\
\
Counter:\n\
  Counter - allows you to count the charged tracks in the event and publishes it to histograms\n\
  in addition it will display the properties of the track like:\n\
  Momentum in x,y,z as well as the charge and the transverse momentum \n\
\n\
\
Display - this allows you to display or hide elements like:\n\
  Vertex: \t\t\tthe point where the collision took place\n\
  Clusters: \t\t\treconstucted points in the detectors showing where the particle was\n\
  Tracks: \t\t\treconstructed trajectories of the particles traversing the detectors\n\
  Show primary tracks only: \twill reduce the number of tracks shown to the ones originating\n\
  \t\t\t\tin the primary vertex\n\
  Geometry: \t\t\tdrawing of ALICE detectors\n\
  Background: \t\t\tchoose the colour of the display panel\n\
  Axes: \t\t\treference system of ALICE Experiment\n\
  (NOTE: The appearance of the object is confirmed by the tick in the checkbox\n\
  on the right)\n\n\
\
Encyclopedia:\n\
  ALICE detector - description of ALICE and its main components;\n\
  (NOTE: click on the pictures of the detectors to get further\n\
  information)\n\n\
\
In the bottom, there are main application control buttons:\n\
  Exit\n\n\n\
YOUR TASK:  \n\
In this part of the master class programm you are supposed to evaluate the mean multiplicity\n\
in pp (first 30 events), peripheral (31st event), semicentral (32nd event) and central (33rd \n\
event) Pb-Pb-collisions and determine the integrated nuclear modification factor for these 3 \n\
Pb-Pb events.\n \n\
But first you need to make yourself familiar with this tool. \n\
In the event-display you have the possibilty to display the clusters (reconstructed space points)\n\
in the different detectors. From these clusters the paths on which a particles went through the \n\
detector are reconstructed - so called tracks. Based on the tracks one can reconstruct the \n\
collision point (primary vertex) from which most of the tracks should come. However as the \n\
some of the detectors are very large one might see several points in which a lot of tracks \n\
originate. The primary collision however should have taken place close to the center of our \n\
coordinate system (X~0, Y~0, Z~0), as then all detectors are able to see the collision and \n\
the tracks can be much better reconstructed. The tracks originating in the other vertices are \n\
called pile-up and do not need to be counted for the multiplicity of the event. Therefore you \n\
can just show the primary tracks (originating in the primary vertex) by clicking on the button \n\
\"Show primary tracks only\". If you click it again you will see the full event again.\n\n\
To achieve the final goal you should use the \"Counter\". This tool will help you count the \n\
number of primary tracks. It will increase the multiplicity in this event by one if you click on \n\
a track, which will then turn red to avoid double counting.\n\
After having clicked the button \"Event analysed\" the multiplicity of this event will be published\n\
to the corresponding histogram in the header \"Event Characteristics\". \n\
In the header \"Event Characteristics\" the histograms are aranged as follows: \n \
top left:\t\t multiplicity per event or as one could also say number of tracks\n \
top right:\t\t multiplicity per event, for tracks with a transverse momentum (pt)\n \
\t\t\t bigger 1.0 GeV/c \n \
bottom left:\t\t transverse momentum distribution for every analysed track \n \
bottom right:\t sign of the charge of every analysed track\n\
The mean multiplicity can be extracted either with a pt-threshold or without and can be seen in the \n\
statistics box of the corresponding histogram in the top row.\
						");
   fFrancais.push_back("Outil d'analyse");
   fEnglish.push_back("Analysis tool");
   fFrancais.push_back("Compteur");
   fEnglish.push_back("Counter");
   fFrancais.push_back("Vous n'avez pas besoin de compter les traces");
   fEnglish.push_back("You do not need to count tracks");
   fFrancais.push_back("Uniquement traces primaires");
   fEnglish.push_back("Show primary tracks only");
   fFrancais.push_back("Caracteristiques de l'evenemet");
   fEnglish.push_back("Event Characteristics");
   fFrancais.push_back("Cette trace a deja ete compte");
   fEnglish.push_back("This track has already been counted");
   fFrancais.push_back("Demarrez d'abord le compteur");
   fEnglish.push_back("Start the Counter first");
   fFrancais.push_back("Nouveau type d'evenement");
   fEnglish.push_back("New event type");

   // MCExeciseR1
   fFrancais.push_back("Analyse du fichier");
   fEnglish.push_back("Analysing file");
   fFrancais.push_back("Systeme en collision ");
   fEnglish.push_back("Collision system");
   fFrancais.push_back("Nombre de traces disponibles");
   fEnglish.push_back("Number of available tracks");
   fFrancais.push_back("Nombre de traces a analyser");
   fEnglish.push_back("Number of tracks to analyze");
   fFrancais.push_back("Nombre d' evenements disponibles");
   fEnglish.push_back("Number of available events");
   fFrancais.push_back("Nombre d' evenements a analyser");
   fEnglish.push_back("Number of events to analyze");
   fFrancais.push_back("Nombre de traces TPC");
   fEnglish.push_back("Number of TPC Tracks");
   fFrancais.push_back("Coups/evt");
   fEnglish.push_back("Counts/evt");
   fFrancais.push_back("Limite inferieure");
   fEnglish.push_back("Low limit");
   fFrancais.push_back("Limite superieure");
   fEnglish.push_back("High limit");
   fFrancais.push_back("Selection interval de centralite");
   fEnglish.push_back("Centrality Interval selection");
   fFrancais.push_back("Interval incorrect");
   fEnglish.push_back("Wrong limits");

   // MCAliCalculator
   fFrancais.push_back("Instructions Calculateur");
   fEnglish.push_back("Calculator Instructions");
   fFrancais.push_back("\
Calculator Instructions \n\n\
\
The Calculator is provided to allow you to calculate rapidity and invariant mass\n\
of the mother partice given the daughter particles characteristics. Is is divided\n\
into 3 segments:\n\n\
\
   Instructions\n\n\
\
   Particle Table - shows various types of particles and their masses, use the table\n\
   to check the identity of the tracks from V0 and Cascade decays\n\n\
\
   Calculator - with:n\n\
      12 Number Fields (4 rows: px, py, pz and mass, 3 columns: (-), (+), Bachelor) - \n\
      the parameters of chosen track will be copied here\n\
      Invariant mass - calculates the invariant mass of the mother particle, the\n\
      result is displayed below the button\n\
      That's a Kaon/Lambda/antiLambda/Xi!- If you have found a particle click on its button\n\
      Clear - Clear all Number Fields \n\
      Load  - Load your results\n\
      Save  - Save your results (autosave at the end of each event)\n\
      Close - close the Calculator\n\n\
\
      ");
   fEnglish.push_back("\
Instructions Calculateur \n\n\
\
Le calculateur vous permet de calculer la rapidite et la masse invariante\n\
de la particule mere connaissant les caracteristiques des particules filles.\n\
Il est divise en 3 segments :\n\n\
\
   Instructions\n\n\
\
   Table des particules - liste divers types de particules avec leur masse, utilisez cette table\n\
   pour verifier l'identite des traces des decroissances V0 et Cascade \n\n\
\
   Calculateur - avec:\n\n\
      12 champs de nombre (4 lignes : px, py, pz et ye, 3 colonnes: (-), (+), Celibataire) - \n\
      les parametres de la trace selectionne seront copies ici\n\
      Masse Invariante - calcule la masse invariantes de la particule mere, le\n\
      resultat est affiche sous le bouton\n\
      Ceci est un Kaon/Lambda/antiLambda/Xi!- Si vous avez trouve une particule cliquez bouton correspondant\n\
      Effacer     - Efface tous les champs de nombre \n\
      Enregistrer - Enregistre vos resultats\n\
      Sauvegarder - Sauvegarde vos resultats (sauvegarde automatique a la fin de chaque evenement)\n\
      Fermer      - Ferme le calculateur\n\n\
\
      ");
   fFrancais.push_back("Tableau des Particules");
   fEnglish.push_back("Particles Table");
   fFrancais.push_back("Type de Particule");
   fEnglish.push_back("Particle Type");
   fFrancais.push_back("Masse [GeV/c2]");
   fEnglish.push_back("Mass [GeV/c2]");
   fFrancais.push_back("Calculateur");
   fEnglish.push_back("Calculator");
   fFrancais.push_back("Celibataire");
   fEnglish.push_back("Bachelor");
   fFrancais.push_back("Masse");
   fEnglish.push_back("Mass");
   fFrancais.push_back("Masse Invariante");
   fEnglish.push_back("Invariant Mass");
   fFrancais.push_back("Ceci est un Kaon");
   fEnglish.push_back("That is a Kaon");
   fFrancais.push_back("Ceci est un Lambda");
   fEnglish.push_back("That is a Lambda");
   fFrancais.push_back("Ceci est un anti-Lambda");
   fEnglish.push_back("That is a anti-Lambda");
   fFrancais.push_back("Ceci est un Xi");
   fEnglish.push_back("That is a Xi");
   fFrancais.push_back("Ceci est du bruit de fond");
   fEnglish.push_back("That is background");
   fFrancais.push_back("Charger");
   fEnglish.push_back("Load");
   fFrancais.push_back("Sauvegarder");
   fEnglish.push_back("Save");

   fFrancais.push_back("Instructions Compteur");
   fEnglish.push_back("Counter Instructions");
   fFrancais.push_back("\
Instructions pour le Compteur\n\n\
\
Le Compteur est un outil pour vous aider a compter les traces primaires dans la fenetre de visualisation.\n\
En cliquant sur les traces la multiplicite est incremente d'une unite. \n\
Les proprietes de la traces sont visualisees :\n\
\t * px \t- quantite de ouvement de la particule dans la direction x \n\
\t * py \t- quantite de ouvement de la particule dans la direction y \n\
\t * pz \t- quantite de ouvement de la particule dans la direction z \n\
\t * pt \t- quantite de ouvement de la particule dans le plan transverse (sqrt(px^2 + py^2)) \n\
\t * charge \t - signe de la charge de la particule \n\
La quantite de mouvement transverse et la charge seront automatiquement ajoutees dans un histogramme\n\
sous l'onglet \"Caracteristiques de l'evenement\".\n\n\
Les deux champs sous les proprietes de la particule indiqueront la multiplicite de l'evenement\n\
(avec ou sans une valeur minimum de pt)\n\n\
Le bouton \"RAZ\"- permet de remmettre a zero tous les champs, cependant le contenu des histogrammes \n\
ne sera pas remis a zero, puisqu'il faudra accumuler les histogrammes pour plusieurs evenments.\n\n\
Le bouton \"Publier Multiplicite\"- ajoutera la multiplicite a l'histogramme correspondant. \n\n\
Le bouton \"Fermer\"-permet de fermer le compteur\n\
				");
   fEnglish.push_back("\
Counter Instructions \n\n\
\
// The Counter is the tool to help you count the primary tracks in the event-display.\n\
By clicking on the tracks the multiplicity is increased by 1 and the properties\
of the track are displayed. \n\
The following properties will be evaluated:\n\
\t *px \t- momentum of the particle in x direction\n\
\t *py \t- momentum of the particle in y direction\n\
\t *pz \t- momentum of the particle in z direction\n\
\t *pt \t- momentum of the particle in the transverse plane (sqrt(px^2 + py^2))\n\
\t *charge \t - sign of the charge of the particle\n\
The transverse momentum (pt) and the charge will be automatically published to a histogram \n\
in the header \"Events Characteristics\".\n\n\
The two fields below the particle properties will give you the multiplicity in the event \n\
(with or without a minimum pt)\n\n\
The \"Clear\"-button - this button will set all the entries to 0, however the entries \n\
in the histograms remain, as they need to be accumulated over several events.\n\n\
The button \"Publish Multiplicity\"- will publish the multiplicity to the corresponding histograms. \n\n\
With the \"Close\"-button you can close the Counter\n");
   fFrancais.push_back("Proprietes de la particule");
   fEnglish.push_back("Particle properties");
   fFrancais.push_back("Proprietes");
   fEnglish.push_back("Properties");
   fFrancais.push_back("Multiplicite");
   fEnglish.push_back("Multiplicity");
   fFrancais.push_back("Multiplicite pour pt > 1.0 GeV/c");
   fEnglish.push_back("Multiplicity for pT > 1.0 GeV/c");
   fFrancais.push_back("Ajouter a l'histogramme Mult");
   fEnglish.push_back("Publish to Mult histogram");
   fFrancais.push_back("Compter toutes les traces primaires");
   fEnglish.push_back("Count all primary tracks");

   fFrancais.push_back("\
Votre tache est de reconstruire le signal de la production des particules J/psi parmi les donnees\n\
enregistrees lors de collisions proton-proton et proton-plomb par le detecteur ALICE au LHC.\n\
Lorsque la particule J/psi est produite dans une collision, elle decroit immediatement en une paire\n\
electron-positron. Ces particules \"filles\" sont detectees par les detecteurs de ALICE. \n\
Pour reconstruite la particule \"mere\", nous faisons appel au calcul de la \"masse invariante\". \n\
Pour une particule donnee, ceci est la masse au repos (3.1 GeV/c^2 pour J/psi).\n\
Lorsque la particule se desintegre, cette quantite est conservee et peut etre calculee a partir de la\n\
quantite de mouvement et de la masse des particules filles. \n\n\
Ainsi donc pour reconstruire les particules J/Psi produites dans les collisions, nous procedons de la\n\
facon suivante : \n\n\
  1.) Selection des electrons et positrons parmi les nombreuses particules produites dans chaque collision\n\
    (e.g. pions, kaons, protons,...). Pour cela nous exploitons les capabilites des detecteurs pour \n\
    identifier les particules, a partie de la mesure de l'energie qu'elles perdent dans la chambre a \n\
    projection temporelle (voir les images). Cependant les electrons peuvent provenir de plusieurs sources \n\
    differentes, par exemple la conversion des photons ou la production directe de paires electron-positron. \n\
    Pour eliminer ces electrons parasites, on choisit d'abord les traces avec une quantite de mouvement \n\
    elevee (pour des considerations cinematiques, la quantite de mouvement devrait etre en moyenne egale a la \n\
    moitie de la masse du J/Psi).\n\
  2.) Association de tous les electrons avec tous les positrons et calcul pour chaque paire leur masse invariante.\n\
    Le signal du J/Psi sera superpose sur un fond combinatoire du a des associations aleatoires. \n\
    Pour soustraire ce fond et obtenir un signal pur, nous devrons etre capable de decrire sa forme. \n\
    Une des methodes consiste a associer des electrons avec des electrons et des positrons avec des positrons \n\
    et calculer la masse invariante de ces paires.\n\
  3.) Repetition de cet exercice pour un grand nombre de collisions (typiquement quelques centaines de millions \n\
    de collisions). \n\
\n\n\
Vous disposez d'un certain nombre d'outils que vous trouverez dans la partie gauche de la fenetre : \n\
  Navigateur d'evenement : permet de naviguer d'un evenement a l'autre.\n\
  Outils d'analyze : \n\
    Cet outil vous permet de : \n\
      - Charger les traces de l'evenement selectionne\n\
      - Examiner la perte d'energie des traces dans la TPC en fonction de leur quantite de mouvement\n\
      - Definir des coupures pour selectioner des traces \n\
      - Appliquer les coupures choisies a la selection des traces \n\
      - Accumuler les distributions de masse invariante pour les paires electron-electron,\n\
        positron-positron et electron-positron \n\
      - extraire le signal en definissant une plage de masses et en comptant les evenements retenus \n\
        apres soustraction du fond.\n\
  Pilotage : \n\
    Vous permet de visualiser ou non les detecteurs ALICE, changer la couleur du fond de l'image, \n\
    remettre a zero le contenu des histogrammes ou redemarrer l'analyze. \n\
  Analyze rapide:\n\
    Apres avoir choisi les coupures sur les traces et les avoir testees sur quelques evenements,\n\
    vous pouvez analyzer rapidement davantage d'evenements en iterant sur un nombre donne \n\
    d'evenements et ainsi incrementer automatiquement les histogrammes.\n\
  Dans la partie principale de la fenetre vous pouvez visualiser les resultats regroupes sous 3 onglets : \n\
  Multi View:\n\
    A partir de cet onglet, vous pouvez visualiser en 3D le detecteur ALICE, avec 2 projections 2D\n\
    distinctes. A l'aide de la souris, vous pouvez zoomer, tourner et deplacer les representations. \n\
  Identification des particules :\n\
    Vous montre dans un histogramme bidimensionnel pour chaque trace sa perte d'energie specifique\n\
    dans le gaz de la TPC en fonction de sa quantite de mouvement. Les lignes indiquent les traces \n\
    selectionnees avec vos coupures choisies.\n\
  Mass invariante : \n\
    Vous montre les distributions de masse invariante pour les paires \n\
      - electron-electron\n\
      - positron-positron\n\
      - electron-positron\n\
    ainsi qu'une distribution pour laquelle les contributions electron-electron et positron-positron \n\
    ont ete soustraites de la distribution electron-positron.");
   fEnglish.push_back("\
Your task is to reconstruct signals of the production of the J/Psi particle in data recorded in\n\
proton-proton or proton-lead collisions with the ALICE detector at the LHC at CERN.\n\
The J/psi particle can be produced in the collision and then immediately decay into a pair\n\
of an electron and a positron. These \"daughter particles we can detect in the detector. \n\
In order to reconstruct the particle they decayed from, the \"mother particle\" we can make use\n\
of the \"invariant mass\". For a single particle, this is just is rest mass (3.1 GeV/c^2 for a J/Psi).\n\
If the particle decays, this quantity is conserved and can  be calculated from the momenta and the\n\
masses of the decay daughters. \n\n\
So in order to reconstruct J/psi particles from collision data we have to do the following:\n\n\
  1.) Select only electrons and positrons out of the many particles that are produced in a collision\n\
    (e.g. pions, kaons, protons,...) For this we can use the particle identification capabilities \n\
    of the detector, based on the specific energy loss in the time projection chamber \n\
    (see attached plots). But even if we only select electrons, they can still come from many\n\
    different sources, for example a conversion of a photon to an electron-positron pair or \n\
    direct production of electron-positron pairs. \n\
    To reject electrons from other sources one possibility is to select only tracks that have a high\n\
    momentum (for kinetic reasons, the average momentum will be somewhere around half the J/psi mass).\n\
  2.)Combine all electrons with all positrons and calculate for each pair the invariant mass.\n\
     The signal of the J/psi will sit on a background from random combinations of electrons and \n\
     positrons. In order to subtract this background and obtain the pure signal, we must be able\n\
     to describe it. One method is to make combinations of electrons with electrons and of positrons\n\
     with positrons and calculate the invariant mass of these pairs.\n\
    \
  3.) Do the same for a large number of collisions. (For publications, we typically analyze some \n\
  hundred million collisions.) \n\
\n\n\
For your task you have several tools at hand, which can be found on the left side of the program:\n\
  Event Navigation: section you can browser through the recorded collisions events.\n\
  Analysis Tools:\n\
    Here you can \n\
      - Load the tracks of the selected event\n\
      - Plot for the tracks the specific energy loss in the TPC as a function of momentum\n\
      - Define cuts to be applied on the tracks \n\
      - Apply the defined cuts on your track selection\n\
      - Fill the invariant mass distributions for the selected tracks for electron-electron,\n\
        positron-positron and electron-positron pairs\n\
      - extract the signal by defining a mass range and counting the entries in the background \n\
        subtracted invariant mass histogramm\n\
  Steering:\n\
    Here you can choose whether to show the ALICE detectors, change the background color, clear all\n\
    filled histograms or restart the analysis from event 1\n\
  Quick Analysis:\n\
  Quick analysis: Once you have found satisfying track cuts and tested them on some events,\n\
    you can analyze more events more quickly by automatically looping over a given number\n\
    of events and filling the histograms automatically.\n\
  On the main part of the window you find several plots and visualizations, grouped in 3 tabs:\n\
  Multi View:\n\
    In this tab you find a 3D representation of the ALICE detector , together with 2 seperate\n\
    2D projections. With your mouse you can zoom, rotate and shift the representations. \n\
    \
  Particle Identification:\n\
    In this tab you find a 2 dimensional histogram which shows you for each track its specific energy\n\
    loss in the TPC as a function of its momentum. Lines indicate which tracks you are selecting with\n\
    your current cut settings.\n\
  Invariant Mass:\n\
    In this tab you find the invariant mass distributions for combinations of \n\
      - electrons with electron\n\
      - positrons with positrons\n\
      - electrons with positrons\n\
    and a plot where the electron-electron and positron positron distribution is subtracted from the \n\
    electron-positron distribution.");
   fFrancais.push_back("Charger traces");
   fEnglish.push_back("Load tracks");
   fFrancais.push_back("Perte d'energie specifique");
   fEnglish.push_back("Specific energy loss");
   fFrancais.push_back("Distribution masse invariante");
   fEnglish.push_back("Invariant Mass Distribution");
   fFrancais.push_back("Mise a jour histogramme PID");
   fEnglish.push_back("Update PID Histogram");
   fFrancais.push_back("Choisir selection traces");
   fEnglish.push_back("Define Track Cuts");
   fFrancais.push_back("Selection traces on/off");
   fEnglish.push_back("Tracks Selection On/Off");
   fFrancais.push_back("Mise a jour masse invariante");
   fEnglish.push_back("Update Invariant Mass Histograms");
   fFrancais.push_back("Extraction du signal");
   fEnglish.push_back("Extract Signal");
   fFrancais.push_back("RaZ histogrammes");
   fEnglish.push_back("Clear Histograms");
   fFrancais.push_back("Recommencer");
   fEnglish.push_back("Restart");
   fFrancais.push_back("Analyse Rapide");
   fEnglish.push_back("Quick Analysis");
   fFrancais.push_back("Trop d'evenements selectionnes");
   fEnglish.push_back("Too many events selected");
   fFrancais.push_back("D'abord activer Analyse Rapide");
   fEnglish.push_back("Enable Quick Analysis first");

   fFrancais.push_back("Selecteur des traces");
   fEnglish.push_back("Tracks Selector");
   fFrancais.push_back("\
   Instructions: Selection des traces \n\n\
      Cet outil permet de selectionner uniquement les traces potentiellement venant  \n\
      d'un electron ou d'un positron et issu de la decroissance d'un J/Psi,\n\n\
      a partir des parametres suivants : \n\
      p     \t- la quantite de mouvement\n\
      dE/dx \t- la perte d'energie specifique de la particule dans le detecteur\n\
      Choississez une valeur minimum et une valeur maximum et cliquez sur \"Engage\".\n\
      Dans l'histogramme correspondant a la perte d'energie specifique que vous trouverez\n\
      dans la fenetre \"Identification des particules\" \n\
      cette selection sera marque par un rectangle. \n\
      Pour ne visualiser dans la representation 3D uniquement  les traces selectionnees \n\
      cliquez \"Traces selectionees\" dans le menu sur la gauche.");
   fEnglish.push_back("\
   Instructions: Track Cuts \n\n\
      The \"Track Cuts\" is the tool that allows you to select only tracks which are \n\
      candidates for being electrons and positrons, originating from a J/psi decay,\n\n\
      based on following quantities:\n\
      p     \t- momentum\n\
      dE/dx \t- specific energy loss of the particle inside the detector\n\
      Enter your desired minimum/maximum values and click on \"Engage\".\n\
      In the \"Specific Energy Loss\" histogram in the \"Particle Identification\" window\n\
      a rectangle which defines the selected area will be drawn. \n\
      If you want to see only the selected tracks in the 3D model, click\n\
      \"Selected Tracks\"in the menu \"Load\" on the left side.");

   fEnglish.push_back("\
     Avec cet outil vous pouvez extraire le signal du J/Psi à partir de la distribution \n\
     de la masse invariante calculee.\n\
     Les parametres suivants sont calcules dans l'intervalle de masses invariantes retenues \n\
      - le signal S par integration de la distribution de masse des paires de signes opposees \n\
      - le bruit de fond B par integration des distributions de masse des paires de memes signes \n\
      - le rapport S/B signal sur bruit de fond \n\
      - la significance définie comme S/sqrt(S+B)\n\
     Les valeurs pour la selection des traces et la plage en masse invariante analysee seront \n\
     sauvegardees avec celles de S, S/B et de la significance dans le fichier \"masterclass.save\"");
   fFrancais.push_back("\
  Instructions: Signal Extraction\n\n\
     With this tool you can extract the J/psi signal from the Invariant mass distribution.\n\
     You can set an invariant mass range from which the signal should be extracted. \n\
     The tool will calculate in the chosen mass range \n\
      - the signal S by integrating the counts in the \"Unlike - Like Sign Mass Distribution\" \n\
      - the background B by integrating the counts in the \"Like Sign Mass Distribution\" \n\
      - the signal over background ratio S/B\n\
      - the significance, defined as S/sqrt(S+B)\n\
     It will also store your current values for the track cuts and the in variant mass window\n\
     together with the obtained signal, S/B and significance in a file called  \"masterclass.save\"");

   fEnglish.push_back("Restart Application"); 
   fFrancais.push_back("Redemarez l'application"); 
}

//=MCMultiLingual========================================================================
MCMultiLingual::~MCMultiLingual()
{
   fEnglish.clear(); 
   fFrancais.clear(); 
}

//=MCMultiLingual========================================================================
MCMultiLingual &MCMultiLingual::Instance()
{
   return fInstance;
}
//=MCMultiLingual========================================================================
void MCMultiLingual::SetLanguage(const TString &lan)
{
   // sets the language
   if (lan.Contains("FR"))
      fLanguage = L_FR;
   else if (lan.Contains("EN"))
      fLanguage = L_EN;
   else
   {
      TString msgText = Form("Unknown Language: %s\n Set To default FR\n", lan.Data());
      new TGMsgBox(gClient->GetRoot(), 0, "Wrong Language", msgText, kMBIconExclamation, kMBOk);
      fLanguage = L_FR;
      return;
   }
}

//=MCMultiLingual========================================================================
char * MCMultiLingual::tr(const char *text)
{
   std::string result = text;
   int index = -1;
   switch (MCMultiLingual::fLanguage)
   {
   case L_FR:
   {
      std::vector<std::string>::iterator itE = std::find(fEnglish.begin(), fEnglish.end(), text);
      if (itE != fEnglish.end())
      {
         index = std::distance(fEnglish.begin(), itE);
         std::vector<std::string>::iterator itF = std::next(fFrancais.begin(), index);
         result = *itF;
      }
      break;
   }
   case L_EN:
   {
      std::vector<std::string>::iterator itF = std::find(fFrancais.begin(), fFrancais.end(), text);
      if (itF != fFrancais.end())
      {
         index = std::distance(fFrancais.begin(), itF);
         std::vector<std::string>::iterator itE = std::next(fEnglish.begin(), index);
         result = *itE;
      }
      break;
   }
   default:
      break;
   }
   char *cstr = new char[result.length() + 1];
   strcpy(cstr, result.c_str());
   return cstr;
}
