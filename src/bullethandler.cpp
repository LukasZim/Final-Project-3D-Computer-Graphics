#include "bullet.cpp"

class BulletHandler {
public:
	BulletHandler(std::string meshpath, std::string texturepath)
		:
		m_mesh(meshpath),
		m_texture(texturepath)
	{
		bullets = {};
	}

	void createBullet(glm::mat4 initialMatrix) {
		Bullet newBullet = Bullet(glm::scale(initialMatrix, glm::vec3(5.0f)), 1, 1, 1);
 		bullets.push_back(newBullet);
	}

	void draw(glm::vec3 playerpos, glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix) {
		std::list<Bullet>::iterator i = bullets.begin();
		while (i != bullets.end()) {
			bool outOfRange = (*i).outOfRange(playerpos);
			if (outOfRange) {
				bullets.erase(i++);
			}
			else {
				(*i).draw(m_projectionMatrix, m_viewMatrix, &m_mesh, &m_texture);
				++i;
			}
		}
	}

	void shadowDraw(glm::mat4 m_projectionMatrix, glm::mat4 m_viewMatrix) {

		std::list<Bullet>::iterator i = bullets.begin();
		while (i != bullets.end()) {
			(*i).shadowDraw(m_projectionMatrix, m_viewMatrix, &m_mesh);
			++i;
		}
	}

private:
	GPUMesh m_mesh;
	Texture m_texture;
	std::list<Bullet> bullets;
};