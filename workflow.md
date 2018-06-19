Workflow basato su branch
=========================

Premessa
--------

Scopo dell’adozione del workflow è attuare un modello di lavoro rigoroso, strutturato e organizzato; scopo della guida è esplicitare come andrà applicato tale workflow. È possibile reperire la descrizione del workflow adottato all’indirizzo <https://nvie.com/posts/a-successful-git-branching-model/>.

Branch
------

La repository è basata su due branch principali, il branch **master** e il branch **develop**. Il branch **master** dovrà trovarsi sempre in uno stato stabile e privo di errori: tutti i merge eseguiti su master dovranno essere prima testati e controllati a fondo per evitare l’introduzione di errori. Il branch **develop**, invece, è il branch di lavoro, e contiene sempre le ultime modifiche apportate al codice: per questo motivo può essere soggetto ad errori.

Comandi base con Git
--------------------

### Visualizzare i branch

Per visualizzare i branch al momento esistenti si fa uso del comando:

```
git branch 
```

L’output del comando visualizzerà la lista dei branch esistenti, mettendo in evidenza il branch corrente.

### Passare ad un altro branch

Per cambiare il branch corrente e passare a un altro branch esistente, si fa uso del comando:

```
git checkout nomebranch 
```

### Creare un nuovo branch

```
git checkout -b nomebranch
```

Questo comando crea il nuovo branch e fa anche il checkout in esso. Se si vuole solamente creare il nuovo branch senza però fare il checkout:

```
git branch nomebranch
```

**N.B.:** il branch qui creato è un branch locale, non fa ancora parte della repository. Perché il nuovo branch sia disponibile anche in remoto, non resta che dare il comando:

```
git push origin nomebranch
```

### Eseguire un merge

Per eseguire un merge è necessario spostarsi (checkout) nel branch in cui si vuole effettuare il merge, e da lì eseguire il merge del branch. Ad esempio, supponiamo di avere due branch, chiamati **A** e **B**. **B** è il branch che contiene le modifiche e che si vuole andare a integrare nel branch principale, mentre **A** è il branch principale in cui si andare ad aggiungere le modifiche effettuate nel branch **B**. I comandi necessari per effettuare quanto precedentemente descritto sono:

```
git checkout A
git merge --no-ff B
```

**Attenzione ai conflitti!** In caso il merge non vada a buon fine e ci siano dei conflitti tra le due versioni, si rimanda alla guida su GitHub[1]. Per semplificare la risoluzione dei conflitti, è possibile utilizzare [2].

### Eliminare un branch

Quando un branch termina la sua utilità, ad esempio quando l’issue (e la ) corrispondente è stata risolta e dopo un eventuale merge, si può voler eliminare un branch. Per fare ciò, si può prima eliminare il branch in locale:

```
git branch -d nomebranch
```

Nel caso in cui il branch fosse effettuato il push in remoto, è il caso di eliminarlo anche da lì:

```
git push origin --delete nomebranch
```

### Struttura dei branch

Come premesso, il workflow in questo progetto sarà un tentativo di adottare il sistema di branching proposto nell’articolo linkato precedentemente. I branch utilizzati saranno, quindi:

-   master;

-   develop;

-   issue;

-   feature;

-   hotfix.

#### master  
Il branch master è il branch principale, e sarà sempre presente per tutta la durata del progetto. Il master deve sempre trovarsi in uno stato stabile e privo di errori.

#### develop  
Il branch develop è il branch di lavoro, quello in cui si trovano sempre le ultime modifiche apportate al progetto. Da questo branch verranno creati tutti gli altri branch (tranne hotfix), e sempre in questo branch quasi tutti gli altri branch confluiranno.

#### issue  
Un branch issue viene creato quando si lavora alla soluzione di una issue aperta, ovviamente i cancelletti vanno sostituiti con il numero della issue corrispondente: ad esempio, se si lavora alla issue42, dovrà essere creato il branch issue42. Un branch issue si stacca sempre dal branch **develop** e in esso deve essere effettuato il merge.

**Esempio:** mi assegno l’issue42 e la risolvo.

```
git checkout develop
git checkout -b issue42
#...modifico il codice per risolvere l'issue..
git checkout develop
git merge --no-ff issue42
git branch -d issue42
```

#### feature  
Un branch feature viene creato per sviluppare una nuova feature al progetto. Come un branch issue, un branch feature si stacca sempre dal branch develop e in esso deve essere effettuato il merge.

#### hotfix  
Il branch hotfix serve per correggere eventuali errori critici presenti sul branch master. Il branch hotfix viene sempre e comunque associato ad un’issue su github (e la ) per completezza e tracciabilità. Il branch hotfix si stacca sempre dal branch master ed esso confluirà (tramite merge) sia nel branch master che nel branch develop.

Note sul workflow
-----------------

Come intuibile dalla descrizione dei branch, quasi ogni branch viene associato all’apertura di una issue su e una corrispondente su . Questo passo è molto importante per permettere di mantenere i problemi e il lavoro tracciati e metodici. Una aperta, con conseguente assegnazione, risoluzione e chiusura, permette di poter sempre risalire al chi, cosa, come, quando e perché di ogni modifica, cioè risalire a:

-   natura del problema in dettaglio;

-   chi se ne occupa;

-   quando se ne occupa;

-   come la risolve.

La tracciabilità del lavoro è di fondamentale importanza ed è uno dei motivi principali che hanno spinto all’adozione di questo workflow.

[1] <https://help.github.com/articles/resolving-a-merge-conflict-using-the-command-line/>

[2] <http://meldmerge.org/>
