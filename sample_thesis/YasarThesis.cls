\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{YasarThesis}

%-------------------------------------------------------------------------------------------------%
%                                  l i n e    s p a c i n g                                       %
%-------------------------------------------------------------------------------------------------%
\newif\if@thdspacing
\@thdspacingfalse
\newif\if@thsspacing
\@thsspacingfalse
\newif\if@thohspacing
\@thohspacingfalse

\DeclareOption{singlespacing}{\@thsspacingtrue
  \PackageInfo{YasarThesis}{Using SINGLE spacing}
}
\DeclareOption{onehalfspacing}{\@thohspacingtrue
  \PackageInfo{YasarThesis}{Using ONE+HALF spacing}
}
\DeclareOption{doublespacing}{\@thdspacingtrue
  \PackageInfo{YasarThesis}{Using DOUBLE spacing}
}

%-------------------------------------------------------------------------------------------------%
%                               d r a f t   o r    f i n a l                                      %
%-------------------------------------------------------------------------------------------------%
\newcommand{\draft@final}{}
\newif\if@thdraft
\@thdrafttrue
\DeclareOption{draft}{
  \@thdrafttrue
  \renewcommand{\draft@final}{draft}
}
\DeclareOption{final}{
  \@thdraftfalse
  \renewcommand{\draft@final}{final}
}

%-------------------------------------------------------------------------------------------------%
%               test if a point size is specified or whether using default                        %
%-------------------------------------------------------------------------------------------------%
\newif\if@thptsize
\@thptsizefalse
\DeclareOption{10pt}{\@thptsizetrue}
\DeclareOption{11pt}{\@thptsizetrue}
\DeclareOption{12pt}{\@thptsizetrue}

%-------------------------------------------------------------------------------------------------%
%                              Switch off some of the book options.                               %
%-------------------------------------------------------------------------------------------------%
\DeclareOption{a5paper}{\OptionNotUsed}
\DeclareOption{b5paper}{\OptionNotUsed}
\DeclareOption{letterpaper}{\OptionNotUsed}
\DeclareOption{legalpaper}{\OptionNotUsed}
\DeclareOption{executivepaper}{\OptionNotUsed}
\DeclareOption{landscape}{\OptionNotUsed}
\DeclareOption{notitlepage}{\OptionNotUsed}
\DeclareOption{twocolumn}{\OptionNotUsed}
\DeclareOption{openany}{\OptionNotUsed}

%-------------------------------------------------------------------------------------------------%
%                              I n v o k e      B o o k    C l a s s                              %
%-------------------------------------------------------------------------------------------------%
% The default action for any option is to pass it to the book class.
\DeclareOption*{\PassOptionsToClass{\CurrentOption}{book}}
 
% Execute the options.
\ExecuteOptions{twoside,onecolumn,final}
\ProcessOptions
 
% Load the book class.
\if@thptsize
  \PackageInfo{YasarThesis}{Options: user specififed point size}
  \LoadClass[a4paper,openright,\draft@final]{book}
\else
  \PackageInfo{YasarThesis}{Options: default 12pt}
  \LoadClass[12pt,a4paper,openright,\draft@final]{book}
\fi

%-------------------------------------------------------------------------------------------------%
%                               R e q u i r e d     P a c k a g e s                               %
%-------------------------------------------------------------------------------------------------%
\RequirePackage{latexsym}
\RequirePackage{graphicx}
\RequirePackage{amsfonts}
\RequirePackage{amssymb}
\RequirePackage{fancyhdr}
\RequirePackage{setspace}

%-------------------------------------------------------------------------------------------------%
%                            s e t     l i n e     s p a c i n g                                  %
%-------------------------------------------------------------------------------------------------%
\onehalfspacing
\if@thsspacing
  \singlespacing
\fi
\if@thohspacing
  \onehalfspacing
\fi
\if@thdspacing
  \doublespacing
\fi

%-------------------------------------------------------------------------------------------------%
%                       s e t    d r a f t    c o p y   w a t e r m a r k                         %
%-------------------------------------------------------------------------------------------------%
\if@thdraft
  \RequirePackage[light,first]{draftcopy}
\fi

\newcounter{subyear}

\setcounter{subyear}{\number\year}

\def\submitted#1{\gdef\@submitted{#1}}

\def\@submittedyear{\ifnum\month>10 \stepcounter{subyear}\thesubyear
  \else\thesubyear\fi}
  
\def\@submittedmonth{\ifnum\month>10 January\else\ifnum\month>8 November
  \else\ifnum\month>6 September\else May\fi\fi\fi}
  
\def\advisor#1{\gdef\@advisor{#1}}
\def\faculty#1{\gdef\@faculty{#1}}
\long\def\@abstract{\@latex@error{No \noexpand\abstract given}\@ehc}

\newcommand*{\makelot}{}
\newcommand*{\makelof}{}
\newcommand*{\makelos}{}
\newcommand*{\begincmd}{
	\doublespacing
	\frontmatter
	\maketitlepage
	\cleardoublepage
	\makekeywords
	\cleardoublepage
	\declaration
	\cleardoublepage
	\makeozet
	\cleardoublepage
	\makeabstract
	\cleardoublepage
	\makeacknowledgements
	\cleardoublepage
	% \makededication\cleardoublepage
	\tableofcontents
	%
	\cleardoublepage
	\addcontentsline{toc}{section}{List of Figures}
	\listoffigures
	\cleardoublepage
	\addcontentsline{toc}{section}{List of Codes}
	\lstlistoflistings
	\cleardoublepage
	%
	\clearpage
	\makelot
	\clearpage
	\makelof
	\clearpage
	\makelos
	\clearpage
	\mainmatter
}



\def\@submitted{\@submittedmonth~\@submittedyear}
\def\dept#1{\gdef\@dept{#1}}
\def\@deptpref{Department of}
\def\departmentprefix#1{\gdef\@deptpref{#1}}
\def\department#1{\gdef\@dept{#1}}
\long\def\acknowledgements#1{\gdef\@acknowledgements{#1}}
\def\dedication#1{\gdef\@dedication{#1}}


%-------------------------------------------------------------------------------------------------%
%                              page size and margin settings                                      %
%-------------------------------------------------------------------------------------------------%
\evensidemargin = 0mm	% Don't change this - use the "oneside" option for single-sided
\oddsidemargin  = 15mm
\textwidth      = 144mm
\topmargin      = 0mm
\headheight     = 10mm
\headsep        = 10mm
\textheight     = 210mm

%-------------------------------------------------------------------------------------------------%
%                              o n e   o r   t w o    s i d e    o p t i o n s                    %
%-------------------------------------------------------------------------------------------------%
\if@twoside
  %%% twoside options %%%
  \PackageInfo{YasarThesis}{set footings: twoside}
  \fancyhf{}
  \fancyhead[le,ro]{\thepage}
  \fancyhead[re]{\sffamily\leftmark}
  \fancyhead[lo]{\sffamily\rightmark}

  \if@thdraft
    \fancyfoot[le]{\textbf{DRAFT}}
    \fancyfoot[ro]{\sffamily\today}
    \renewcommand{\footrulewidth}{0.4pt}
  \fi
\else
  %%% oneside - only for submission to external examiners
  \fancyhf{}
  \fancyhead[l]{\sffamily\leftmark}
  \fancyhead[c]{}
  \fancyhead[r]{\sffamily\thepage}

  \if@thdraft
    \fancyfoot[l]{\textbf{DRAFT}} 
    \fancyfoot[c]{}
    \fancyfoot[r]{\sffamily \today}
    \renewcommand{\footrulewidth}{0.4pt}
  \fi
\fi
\renewcommand{\headrulewidth}{0.4pt}


%-------------------------------------------------------------------------------------------------%
%  R e d e f i n e   f r o n t m a t t e r ,  m a i n m a t t e r   a n d   b a c k m a t t e r   %
%-------------------------------------------------------------------------------------------------%
% frontmatter, mainmatter and backmatter from book.cls have been copied and altered
\renewcommand\frontmatter{
  \cleardoublepage
  \@mainmatterfalse
  \pagenumbering{roman}
  \pagestyle{plain}
  % frontmatter chapter style 
  \makeatletter
  \def\@makechapterhead##1{
    \vspace*{50\p@}
    {\parindent \z@ \raggedright \normalfont
      \ifnum \c@secnumdepth >\m@ne
        \if@mainmatter
          \huge\bfseries \@chapapp\space \thechapter
          \par\nobreak
          \vskip 10\p@
        \fi
      \fi
      \normalfont
%     \noindent\rule[3mm]{\textwidth}{1mm}
      \vskip 10\p@
      \interlinepenalty\@M
      \Huge \bfseries ##1\par\nobreak
      \vskip 40\p@
    }}
  \makeatother

  \addcontentsline{toc}{chapter}{Front Matter}
}
\renewcommand\mainmatter{%
  \cleardoublepage
  \@mainmattertrue
  \pagenumbering{arabic}
  \pagestyle{fancy}
  \renewcommand{\chaptermark}[1]{\markboth {\@chapapp\ \thechapter.\ ##1}{}}
  \renewcommand{\sectionmark}[1]{\markright{\thesection.\ ##1}{}}

  % mainmatter chapter style 
  \makeatletter
  \def\@makechapterhead##1{%
    \vspace*{50\p@}%
    {\parindent \z@ \raggedright \normalfont
      \ifnum \c@secnumdepth >\m@ne
        \if@mainmatter
          \huge\bfseries \@chapapp\space \thechapter
          \par\nobreak
          \vskip 10\p@
        \fi
      \fi
      \normalfont
      \noindent\rule[3mm]{\textwidth}{1mm}
      \vskip 10\p@
      \interlinepenalty\@M
      \Huge \bfseries ##1\par\nobreak
      \vskip 40\p@
    }}
  \makeatother
}

%\renewcommand\backmatter{%
%  \if@openright
%    \cleardoublepage
%  \else
%    \clearpage
%  \fi
%  \@mainmatterfalse}

%-------------------------------------------------------------------------------------------------%
%                                     m a k e   t i t l e                                         %
%  Sourced from Andrew Clark                                                                      %
%-------------------------------------------------------------------------------------------------%
\newcommand*{\maketitlepage}{
	{
	%\newpage
	%
	%\addcontentsline{toc}{section}{maketitlepage}
	%
	\thispagestyle{empty}
	\begin{center}
		\includegraphics[width=0.3\textwidth]{yasar.eps}
	\end{center}
	\vspace*{0in}
	\begin{center}
		\LARGE\@title
	\end{center}
	\vspace{.6in}
	\begin{center}
		\@author
	\end{center}
	\vspace{.6in}
	\vspace{.3in}
	\begin{center}
		\@dept\\
		\@faculty\\
		Advisor:~\@advisor
	\end{center}
	\vspace{.3in}
	\begin{center}
		\@submitted
	\end{center}
 	}
	\cleardoublepage
}
  

%-------------------------------------------------------------------------------------------------%
%                                        k e y w o r d s                                          %
%-------------------------------------------------------------------------------------------------%
\newcommand*{\makekeywords}{
	\newpage
	\addcontentsline{toc}{section}{Keywords}
	\begin{center}
		\Large\textbf{Keywords}
	\end{center}
	\@keywords
	\cleardoublepage
}

%-------------------------------------------------------------------------------------------------%
%                                        a b s t r a c t                                          %
%-------------------------------------------------------------------------------------------------%
\newcommand*{\makeabstract}{
	\newpage
	\addcontentsline{toc}{section}{Abstract}
	\begin{center}
		\Large\textbf{Abstract}
	\end{center}
	\@abstract
	\cleardoublepage
}

\newcommand*{\makeozet}{
	\newpage
	\addcontentsline{toc}{section}{\"{O}z}
	\begin{center}
		\Large\textbf{\"{O}z}
	\end{center}
	\@ozet
	\cleardoublepage
}

%-------------------------------------------------------------------------------------------------%
%                                     d e c l a r a t i o n                                       %
%-------------------------------------------------------------------------------------------------%
\newcommand{\declaration}{
	\chapter*{Declaration}
	\addcontentsline{toc}{section}{Declaration}
	
	The work contained in this thesis has not been previously submitted
	for a degree or diploma at any higher education institution.  To the
	best of my knowledge and belief, the thesis contains no material
	previously published or written by another person except where due
	reference is made.
	
	\vspace{15mm}
	
	\noindent
	\textbf{Signed:}\makebox[80mm]{\dotfill} 
	\textbf{Date:}\makebox[40mm]{\dotfill}
	
	\newpage
}

%-------------------------------------------------------------------------------------------------%
%                                     a c k n o w l e d g e                                       %
%-------------------------------------------------------------------------------------------------%

\def\makeacknowledgements{
  \ifx\@acknowledgements\undefined
  \else
    \addcontentsline{toc}{section}{Acknowledgements}
    \begin{center}
      \Large \textbf{Acknowledgements}
    \end{center}
    \@acknowledgements
    \clearpage
  \fi
  }
% \DeclareOption{myorder}{
%   \renewcommand*{\begincmd}{\doublespacing}}
% \DeclareOption{lot}{\renewcommand*{\makelot}{
%   \addcontentsline{toc}{section}{List of Tables}\listoftables}}
% \DeclareOption{lof}{\renewcommand*{\makelof}{
%   \addcontentsline{toc}{section}{List of Figures}\listoffigures}}
% \DeclareOption{los}{
%   \renewcommand*{\makelos}{
%     \RequirePackage{losymbol}
%     \section*{List of Symbols\@mkboth {LIST OF SYMBOLS}{LIST OF SYMBOLS}}
%     \@starttoc{los}
%     \addcontentsline{toc}{section}{List of Symbols}
%   }
% }

\long\def\ozet#1{\gdef\@ozet{#1}}
\long\def\keywords#1{\gdef\@keywords{#1}}
\long\def\abstract#1{\gdef\@abstract{#1}}
\AtBeginDocument{\begincmd}
\endinput


%-------------------------------------------------------------------------------------------------%
%                                    p u b l i c a t i o n s                                      %
%-------------------------------------------------------------------------------------------------%
\newenvironment{publications}[2][pub-unsrt]{% second param is name of bib file containing publctns
\chapter*{Previously Published Material}
\addcontentsline{toc}{section}{Previously Published Material}

\pubinit{}

\pubbibliographystyle{#1}
\pubbibliography{#2}
}
{
\pubclose{}
}

\newenvironment{publications*}[2][pub-unsrt]{% second parameter is name of bib file containing publications
%\chapter{Previously Published Material}

\pubinit{}

\pubbibliographystyle{#1}
\pubbibliography{#2}
}
{
\pubclose{}
}

%-------------------------------------------------------------------------------------------------%
%                          p u b l i c a t i o n s   c o m m a n d s                              %
%-------------------------------------------------------------------------------------------------%
%%% Read publication .aux file here in the preamble: 
%   Make \bibcite use prefix 'p' (not 'b') and input publication .aux file
%   In this way, labels of the form p@xxxxx are created for publications
%
\def\bibcite{\@newl@bel p}
\@input{pub-\jobname.aux}%
\def\bibcite{\@newl@bel b}
\expandafter\let\csname p@*\endcsname\@empty % define the wildcard '*' label

\newcommand{\pubinit}{
%  \if@filesw
    \newwrite\@pubmainaux
    \immediate\openout\@pubmainaux pub-\jobname.aux
    \immediate\write\@pubmainaux{\relax}%

    %% save current \auxout and set it to current pub aux file
    %% Then you can use the standard \@cite command used below in \@citexpub
    \let\@auxoutx=\@auxout
    \let\@auxout=\@pubmainaux
 
%  \fi
}

\newcommand{\pubclose}{
  \let\@auxout=\@mainaux
}

\newcommand{\pubbibliography}[1]{
  \if@filesw
    \immediate\write\@pubmainaux{\string\bibdata{#1}}%
  \fi
}

\newcommand{\pubbibliographystyle}[1]{
  \if@filesw
     \immediate\write\@pubmainaux{\string\bibstyle{#1}}%
  \fi
}

\newcommand{\listpublications}{
  \@input@{pub-\jobname.bbl}
}

%%%  based on \nocite but use prefix p not b. Also use \@pubmainaux not \auxout
\def\nocitepub#1{
  \@bsphack
  \@for\@citeb:=#1\do{%
    \edef\@citeb{\expandafter\@firstofone\@citeb}%
    \if@filesw
       \immediate\write\@pubmainaux{\string\citation{\@citeb}}
    \fi
    \@ifundefined{p@\@citeb}{\G@refundefinedtrue
        \@latex@warning{Publication Citation `\@citeb' undefined}}{}
  }%
  \@esphack
}

\DeclareRobustCommand\citepub{%
  \@ifnextchar [{\@tempswatrue\@citexpub}{\@tempswafalse\@citexpub[]}}

%%%  based on \@citepub but use prefix p not b. Also use \@pubmainaux not \auxout
\def\@citexpub[#1]#2{%
  \let\@citea\@empty
  \@cite{\@for\@citeb:=#2\do%
    {\@citea\def\@citea{,\penalty\@m\ }%
     \edef\@citeb{\expandafter\@firstofone\@citeb\@empty}%
     \if@filesw%
        \immediate\write\@pubmainaux{\string\citation{\@citeb}}%
     \fi%
     \@ifundefined{p@\@citeb}%
       {%
         \mbox{\reset@font\bfseries ?}%
         \G@refundefinedtrue%
         \@latex@warning{Publication Citation `\@citeb' on page \thepage \space undefined}%
       }%
       {\hbox{\csname p@\@citeb\endcsname }}}}{#1}
}

\newenvironment{thepubbibliography}[1]
     {\list{\@biblabel{\@arabic\c@enumiv}}%
           {\settowidth\labelwidth{\@biblabel{#1}}%
            \leftmargin\labelwidth
            \advance\leftmargin\labelsep
            \@openbib@code
            \usecounter{enumiv}%
            \let\p@enumiv\@empty
            \renewcommand\theenumiv{\@arabic\c@enumiv}}%
      \sloppy
      \clubpenalty4000
      \@clubpenalty \clubpenalty
      \widowpenalty4000%
      \sfcode`\.\@m}
     {\def\@noitemerr
       {\@latex@warning{Empty `thebibliography' environment}}%
      \endlist
} % thepubbibloigraphy
