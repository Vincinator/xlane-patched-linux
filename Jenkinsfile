
pipeline {

    agent any
    stages {
        stage('Make asgard Kernel (deb pkg)') {
            steps {
                
                sh 'rm ../*.changes || echo "no .changes files to delete.."'
                sh 'rm asgard-bin/* || echo "no build files to delete.."'
                sh 'mkdir -p asgard-bin/'
                sh 'cp asgard-kernel.config .config'
                sh 'make bindeb-pkg LOCALVERSION= KDEB_PKGVERSION= ARCH=x86 -j 56'
                sh 'rm -f asgard-bin/*'
                sh 'mv ../*.deb asgard-bin/'
                sh 'cp install_asgard_kernel.sh asgard-bin/'
                archiveArtifacts 'asgard-bin/*.deb'
                archiveArtifacts 'asgard-bin/*.sh'
              }
        }
    }
}