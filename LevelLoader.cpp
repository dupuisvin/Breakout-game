#include "LevelLoader.h"
#include "TextureLoader.h"
#include "Position.h"
#include "Sprite.h"
#include "Player.h"
#include "Ball.h"
#include "Brick.h"
#include "Active.h"
#include "CollisionAABB.h"
#include "CollisionCircle.h"
#include "PaddleCollisionObj.h"

#include "rapidjson/document.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/encodedstream.h"

#include <entt/entt.hpp>
#include <errno.h>
#include <stdexcept>

using namespace SDLEngine;
using namespace Breakout;

LevelBreakout LevelLoader::LoadLevel(std::string filename, entt::registry& reg, SDL_Renderer *renderer)
{
	LevelBreakout ret;

	char readBuffer[256]; //@todo Check if this buffer is enough
	FILE* fp = nullptr;
	auto err = fopen_s(&fp, filename.c_str(), "rb");
	if (err == 0)
	{
		rapidjson::FileReadStream bis(fp, readBuffer, sizeof(readBuffer));
		rapidjson::AutoUTFInputStream<unsigned, rapidjson::FileReadStream> eis(bis);

		rapidjson::Document doc;
		doc.ParseStream<0, rapidjson::AutoUTF<unsigned>>(eis);

		if (!doc.HasParseError())
		{
			const auto& background = doc["background"];
			ret.BackgroundEntity = reg.create();
			reg.emplace<Position>(ret.BackgroundEntity, 0.0f, 0.0f);
			reg.emplace<Sprite>(ret.BackgroundEntity, TextureLoader::LoadFromFile(renderer, background.GetString(), 0));
			reg.emplace<Active>(ret.BackgroundEntity);

			const auto& paddle = doc["paddle"];
			ret.PaddleEntity = reg.create();
			reg.emplace<Position>(ret.PaddleEntity, 0.0f, 0.0f);
			auto paddleSprite = TextureLoader::LoadFromFile(renderer, paddle.GetString(), 1);
			int paddleHalfHeight = paddleSprite.Rect.h / 2;
			auto paddleCenterEntity = reg.create();
			reg.emplace<CollisionAABB>  (
				paddleCenterEntity,
				paddleSprite.Rect.w - paddleHalfHeight * 2.0f, 
				static_cast<float>(paddleSprite.Rect.h), 
				glm::vec2(static_cast<float>(paddleHalfHeight), 0.0f));
			auto paddleLeftEntity = reg.create();
			reg.emplace<CollisionCircle>(paddleLeftEntity, static_cast<float>(paddleHalfHeight), glm::vec2(paddleHalfHeight, paddleHalfHeight));
			auto paddleRightEntity = reg.create();
			reg.emplace<CollisionCircle>(paddleRightEntity, static_cast<float>(paddleHalfHeight), glm::vec2(paddleSprite.Rect.w-paddleHalfHeight, paddleHalfHeight));
			reg.emplace<PaddleCollisionObj>(ret.PaddleEntity, paddleCenterEntity , paddleLeftEntity, paddleRightEntity);
			reg.emplace<Sprite>(ret.PaddleEntity, std::move(paddleSprite));
			reg.emplace<Player>(ret.PaddleEntity);
			reg.emplace<Active>(ret.PaddleEntity);

			const auto& ball = doc["ball"];
			ret.BallEntity = reg.create();
			reg.emplace<Position>(ret.BallEntity, 0.0f, 0.0f);
			auto ballTexture = TextureLoader::LoadFromFile(renderer, ball.GetString(), 1);
			reg.emplace<CollisionCircle>(ret.BallEntity, ballTexture.Rect.w / 2.0f, glm::vec2(ballTexture.Rect.w / 2.0f, ballTexture.Rect.h / 2.0f));
			reg.emplace<Sprite>(ret.BallEntity, std::move(ballTexture));
			reg.emplace<Ball>(ret.BallEntity);
			reg.emplace<Active>(ret.BallEntity);

			const auto& music = doc["music"];
			ret.LevelMusicPath = music.GetString();

			const rapidjson::Value& bricks = doc["bricks"];
			ret.BricksEntities.reserve(bricks.Size());
			for (rapidjson::Value::ConstValueIterator itr = bricks.Begin(); itr != bricks.End(); ++itr)
			{
				rapidjson::Value::ConstMemberIterator x = itr->FindMember("x");
				rapidjson::Value::ConstMemberIterator y = itr->FindMember("y");
				rapidjson::Value::ConstMemberIterator color = itr->FindMember("color");

				auto brick = reg.create();
				ret.BricksEntities.push_back(brick);
				reg.emplace<Position>(brick, static_cast<float>(x->value.GetInt()), static_cast<float>(y->value.GetInt()));
				auto brickTexture = TextureLoader::LoadFromFile(renderer, color->value.GetString(), 1);
				reg.emplace<CollisionAABB>(brick, static_cast<float>(brickTexture.Rect.w), static_cast<float>(brickTexture.Rect.h), glm::vec2());
				reg.emplace<Sprite>(brick, std::move(brickTexture));
				reg.emplace<Brick>(brick);
				reg.emplace<Active>(brick);
			}
		}
		else
			throw std::runtime_error("Parse error while reading level file : " + filename);
	}
	else
		throw  std::runtime_error("Failed to open level file");

	if (fp)
	{
		if(fclose(fp) != 0)
			throw  std::runtime_error("Failed to close level file");
	}

	return ret;
}