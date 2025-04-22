pipeline {
    agent any  // Can run on any available agent

    environment {
        PACKAGE_DIR = "node_package"   // Directory to hold the node files
        LABEL_FILE = "label"           // Path to the label file
        CFG_FILE = "cfg.xml"           // Path to the config file
        SO_FILE = "app_bln.so"         // Path to the compiled shared object file
        BUILDINFO_FILE = "app_bln.so.buildinfo"  // Path to the build info file
        PACKAGE_NAME = "app_bln.el"    // Name of the final EL package
        GIT_REPO = 'https://github.com/Anirudhmdmd/web1.git'  // Git repository URL
        BRANCH = 'main'  // Branch to checkout (e.g., main, master, etc.)
    }

    stages {
        stage('Checkout') {
            steps {
                // Checkout the code from GitHub repository
                git branch: "${BRANCH}", url: "${GIT_REPO}"
            }
        }

        stage('Preparation') {
            steps {
                // Clean up any previous workspace files
                deleteDir()
            }
        }

        stage('Build Node Package') {
            steps {
                script {
                    // Create the package directory
                    bat "mkdir -p ${PACKAGE_DIR}"

                    // Copy the required files to the package directory
                    bat "cp ${LABEL_FILE} ${PACKAGE_DIR}/"
                    bat "cp ${CFG_FILE} ${PACKAGE_DIR}/"
                    bat "cp ${SO_FILE} ${PACKAGE_DIR}/"
                    bat "cp ${BUILDINFO_FILE} ${PACKAGE_DIR}/"

                    // Create the tar.gz package
                    bat "tar -czvf ${PACKAGE_NAME}.tar.gz -C ${PACKAGE_DIR} ."
                }
            }
        }

        stage('Archive Package') {
            steps {
                // Archive the generated package as a build artifact
                archiveArtifacts artifacts: "${PACKAGE_NAME}.tar.gz", allowEmptyArchive: false
            }
        }
    }

    post {
        always {
            // Clean workspace after the job is done
            cleanWs()
        }
    }
}
