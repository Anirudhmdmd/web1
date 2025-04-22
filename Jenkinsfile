pipeline {
    agent any
    stages {
        stage('Checkout') {
            steps {
                git url: 'https://github.com/Anirudhmdmd/web1.git', branch: 'main'
            }
        }

        stage('Build Node Package') {
            steps {
                script {
                    bat 'if not exist node_package mkdir node_package'
                    bat 'dir > dir_contents.txt'
                    bat '''
                    if exist label (
                        copy label node_package\\
                    ) else (
                        echo ERROR: label file not found!
                        exit /b 1
                    )
                    '''
                }
            }
        }

        stage('Archive Package') {
            when {
                expression {
                    return fileExists('node_package')
                }
            }
            steps {
                archiveArtifacts artifacts: 'node_package/**', fingerprint: true
            }
        }
    }
    post {
        always {
            cleanWs()
        }
    }
}
