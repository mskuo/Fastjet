#include <TFile.h>
#include <TH1D.h>
#include <TTree.h>
#include <TBranch.h>
#include <TLeaf.h>
#include <fastjet/ClusterSequence.hh>
using namespace fastjet ;

#include <iostream>
using namespace std ;

int main()
{
	//Load the data
	TFile *in = TFile::Open("myfile.root");
	//create the new file to store the result
	TFile *out = new TFile("out.root" , "recreate" ) ;
	TTree *Candidates ;

	int n_particles , njets , nsparticles[3000] , label[3000] ;
	double pt[3000], y[3000], phi[3000] ;
	double spt[3000], sy[3000] , sphi[3000] ;

	//result Tree
	TTree *mytree  = new TTree("Jets" , "Jets" ) ;
	mytree -> Branch ("n_particles" , &n_particles , "n_particles/I" ) ;
	mytree -> Branch ("njets" , &njets , "njets/I" );
	mytree -> Branch ("pt" , pt , "pt[njets]/D" ) ;
	mytree -> Branch ("y" , y , "y[njets]/D" ) ;
	mytree -> Branch ("phi" , phi , "phi[njets]/D" ) ; 	
	mytree -> Branch ("nsparticles" , nsparticles , "nsparticles[njets]/I") ;



	//For single particles in jets
	mytree -> Branch ("label" , label , "label[n_particles]/I" ) ;
	mytree -> Branch ("spt" , spt , "spt[n_particles]/D" ) ;
	mytree -> Branch ("sy" , sy , "sy[n_particles]/D" ) ;
	mytree -> Branch ("sphi" , sphi , "sphi[n_particles]/D" ) ;	


	//Load the data into Candidates
	in -> GetObject( "Candidates" , Candidates ) ;
	int nparticles ;
	double e[3000], px[3000], py[3000], pz[3000] ;
	Candidates -> SetBranchAddress("nparticles" , &nparticles) ;
	Candidates -> SetBranchAddress("e",e);
	Candidates -> SetBranchAddress("px",px);
	Candidates -> SetBranchAddress("py",py);
	Candidates -> SetBranchAddress("pz",pz);

	//loop for all event
	for ( int evt = 0 ; evt < Candidates->GetEntries() ; evt++ )
	{
		Candidates -> GetEntry(evt) ;
		vector<PseudoJet> particles  ;
		//loop for all particles in an event 
		for (int i=0 ; i < nparticles ; i++ )
		{
			particles.push_back( PseudoJet( px[i], py[i], pz[i], e[i] )) ;
		}

		//choose a jet definition
		double R = 0.5 ;
		JetDefinition jet_def(antikt_algorithm, R ) ;
		
		//run the clustering, extract the jets
		ClusterSequence cs(particles , jet_def) ;
		vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets()) ;

		n_particles = nparticles ;
		int counter = 0 ;
		//store the result jets
		njets = jets.size() ;
		for (int j=0 ; j < njets ; j++ )
		{
			//store the four-momentum of jets
			pt[j] = jets[j].pt() ;
			y[j] = jets[j].rap() ;
			phi[j] = jets[j].phi() ;
			vector<PseudoJet> constituents = jets[j].constituents() ;
			nsparticles[j] = constituents.size() ;
			for ( int k=0 ; k < nsparticles[j] ; k++ )
			{
				//store the four-momentum of single particle in a jet
				label[counter]= j ;
				spt[counter]  = constituents[k].pt() ;
				sy[counter]   = constituents[k].rap() ;
				sphi[counter] = constituents[k].phi() ;
				counter++ ;
			}

		}
		mytree -> Fill() ;	
	}

	in -> Close() ;
	out -> Write() ;
	out -> Close() ;
}
